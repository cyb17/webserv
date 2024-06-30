/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:39:42 by yachen            #+#    #+#             */
/*   Updated: 2024/06/30 17:04:55 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigExtractor.hpp"
#include <cstdlib>
#include <iostream>

ConfigExtractor::ConfigExtractor() {}

ConfigExtractor::~ConfigExtractor()	{}

Location*				ConfigExtractor::fillLocationData( std::vector<Token*>& tokens )
{
	Location*	location = new Location;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "autoindex")
			location->autoindex = tokens[++i]->value;
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "root")
		{
			location->root = tokens[++i]->value;
			if (tokens[i + 1 ]->type == PARAMETER)
				location->index = tokens[++i]->value;
		}
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "allowMethods")
		{
			while (tokens[++i]->type == PARAMETER)
				location->allowMethods.push_back( tokens[i]->value );
			--i;
		}
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "return")
		{
			location->redirection.first = std::atoi(tokens[++i]->value.c_str());
			location->redirection.second = tokens[++i]->value;
		}
	}
	// std::cout << "-------------------\n";
	// 	std::cout << "autoindex: " << location->autoindex << "\n";
	// 	std::cout << "root: " << location->root << "\n";
	// 	if (!location->index.empty())
	// 		std::cout << "index: " << location->index << "\n";
	// 	std::cout << "allowMethods: \n";
	// 	for (size_t j = 0; j < location->allowMethods.size(); ++j)
	// 		std::cout << location->allowMethods[j] << " ";
	// 		std::cout << '\n';
		return location;
}

std::map<int, std::string>	ConfigExtractor::fillErrorPageData( std::vector<Token*>& tokens )
{
	std::map<int, std::string>	errorPages;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		int code = std::atoi(tokens[i]->value.c_str());
		errorPages[code] = tokens[++i]->value;
	}
	return errorPages;
}

Server*					ConfigExtractor::fillServerData( std::vector<Token*>& tokens )
{
	Server*	server = new Server;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "listen")
			server->listen = tokens[++i]->value;
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "serverName")
			server->serverName = tokens[++i]->value;
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "host")
			server->host = tokens[++i]->value;
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "clientMaxBodySize")
			server->clientMaxBodySize = std::atoi(tokens[++i]->value.c_str());
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "root")
		{
			server->root = tokens[++i]->value;
			if (tokens[i + 1 ]->type == PARAMETER)
				server->index = tokens[++i]->value;
		}
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "errorPages")
		{
			std::vector<Token*>	errorPages;
			while (tokens[++i]->type != DIRECTIVE)
			{
				if (tokens[i]->type == PARAMETER)
					errorPages.push_back( *(tokens.begin() + i) );
			}
			--i;
			server->errorPages =  fillErrorPageData( errorPages );
		}
		else if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "location")
		{
			std::vector<Token*>	location;
			while (tokens[++i]->type != BRACE_CL)
			{
				if (tokens[i]->type == PARAMETER || tokens[i]->type == DIRECTIVE)
					location.push_back( *(tokens.begin() + i) );
			}
			// for (size_t i = 0; i < location.size(); ++i)
				// std::cout << location[i]->value << '\n';
			server->location.push_back( fillLocationData( location ) );
		}
	}
	return server;
}


void					ConfigExtractor::fillServerList( std::vector<Token*>& tokens )
{
	size_t size = tokens.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "server")
		{
			std::vector<Token*>	list;
			while (++i < size)
			{
				if (tokens[i]->type == DIRECTIVE && tokens[i]->value == "server")
					break;
				list.push_back( *(tokens.begin() + i) );
			}
			// for (size_t i = 0; i < list.size(); ++i)
				// std::cout << list[i]->value << '\n';
			_serverList.push_back( fillServerData( list ) );
			--i;
		}
	}
	printServerList();
}

std::vector<Server*>&	ConfigExtractor::getServerList()
{
	return _serverList;
}

using std::cout;

void					printLocation( std::vector<Location*>& location )
{
	for (size_t i = 0; i < location.size(); ++i)
	{
		cout << "---------locations : \n";
		cout << "autoindex: " << location[i]->autoindex << "\n";
		cout << "root: " << location[i]->root << "\n";
		if (!location[i]->index.empty())
			cout << "index: " << location[i]->index << "\n";
		cout << "allowMethods: ";
		for (size_t j = 0; j < location[i]->allowMethods.size(); ++j)
			cout << ' ' << location[i]->allowMethods[j] << " ";
		cout << '\n';
		cout << "code redir: " << location[i]->redirection.first << " path: " << location[i]->redirection.second << '\n';
	}
}

void					ConfigExtractor::printServerList()
{
	cout << "nb of server: " << _serverList.size() << '\n';
	for (size_t i = 0; i < _serverList.size(); ++i)
	{
		cout << "serverName: " << _serverList[i]->serverName << '\n';
		cout << "listen: " << _serverList[i]->listen << '\n';
		cout << "host: " << _serverList[i]->host << '\n';
		cout << "root: " << _serverList[i]->root << '\n';
		cout << "index: " << _serverList[i]->index << '\n';
		cout << "clientMaxBodySize: " << _serverList[i]->clientMaxBodySize << '\n';
		cout << "---------errorPages : \n";
		for (std::map<int, std::string>::iterator it = _serverList[i]->errorPages.begin(); it != _serverList[i]->errorPages.end(); ++it)
			cout << "code: " << it->first << " path: " << it->second << '\n';
		printLocation( _serverList[i]->location );
	}
}