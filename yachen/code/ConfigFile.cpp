/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:39:38 by yachen            #+#    #+#             */
/*   Updated: 2024/06/26 17:31:29 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

ConfigFile::ConfigFile( char* path ) : _path( path ) {}

ConfigFile::~ConfigFile()
{
	for (size_t i = 0; i < _tokenList.size(); ++i)
		delete _tokenList[i];
	_tokenList.clear();
}

Token*	makeToken( std::string& value )
{
	std::string	directives[11] = { "server", "listen", "host", "serverName", "clientMaxBodySize",\
	 "root", "errorPages", "location", "allowMethods", "autoindex", "return" };
	Token* token = new Token;
	for (int i = 0; i < 11; i++)
	{
		if (value == directives[i])
		{
			token->type = DIRECTIVE;
			token->value = value;
			return token;
		}
	}
	if (value == "{")
		token->type = BRACE_OP;
	else if (value == "}")
		token->type = BRACE_CL;
	else
		token->type = PARAMETER;
	token->value = value;
	return token;
}

// lit et decompose le contenu du fichier de configuration en une list de tokens
void	ConfigFile::makeTokenList()
{
	std::string	line;
	std::fstream	fd( _path, std::fstream::in );
	if (!fd.is_open())
		throw std::invalid_argument( "open configuration file failed" );
	while (std::getline( fd, line ))
	{
		std::istringstream	ss( line );
		std::string	str;
		if (!(ss >> str))
			continue;
		else
		{
			_tokenList.push_back( makeToken( str ) );
			while ( ss >> str )
				_tokenList.push_back( makeToken( str ));
		}
	}
	fd.close();
	std::cout << "------------------------------print tokenlist block--------------------------------------\n";
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{
		if ((*it)->type == 1)
			std::cout << "type : DIRECTIVE"; 
		else if ((*it)->type == 2)
			std::cout << "type : PARAMETER"; 
		else if ((*it)->type == 3)
			std::cout << "type : BRACE_OP"; 
		else if ((*it)->type == 4)
			std::cout << "type : BRACE_CL";
		std::cout << "  |   value: " << (*it)->value << "\n";
	}
	std::cout << "--------------------------------------------------------------------\n";
}

// verifie le {} sont correctement placees et fermees,
void	ConfigFile::checkBrace()
{
	int	openBrace = 0;
	int	closeBrace = 0;
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{	
		std::cout << (*it)->value << '\n';
		if ((*it)->type == BRACE_OP)
		{
			if (it - 1  == _tokenList.begin())
				throw std::invalid_argument( "1incorrect open brace position detected" );
			if ((*(it - 1))->type != DIRECTIVE)
				throw std::invalid_argument( "incorrect open brace position detected" );
				// || ((*(it - 1))-> type == DIRECTIVE
					// && (*(it - 1))->value != "server" && (*(it - 1))->value != "location" && (*(it - 1))->value != "errorPages"))
			openBrace++;
		}
		if ((*it)->type == BRACE_CL)
		{
			if (it + 1 != _tokenList.end() && ((*(it + 1))-> type != DIRECTIVE
				|| ((*(it + 1))-> type == DIRECTIVE
					&& (*(it + 1))->value != "server" && (*(it + 1))->value != "location" && (*(it + 1))->value != "errorPages")))
						throw std::invalid_argument( "incorrect close brace position detected" );
				closeBrace++;
		}
	}
	if (openBrace != closeBrace)
		throw std::invalid_argument( "brace not closed" );
}

// void	ConfigFile::checkLocationContent( std::vector<Token*>& location )
// {
// 	std::string	mandatoryDirectives[3] = { "allowMetods", "autoindex", "return" };
	
// }

// void	ConfigFile::checkServerContent( std::vector<Token*>& server )
// {
// 	std::string	mandatoryDirectives[7] = { "listen", "host","serverName", "clientMaxBodySize",\
// 	 "root", "errorPages", "location" };
// 	std::map<std::string, int>	directivePrensent;
// 	for (int i = 0; i < 7; i++)
// 		directivePrensent.insert( std::pair<std::string, int>(mandatoryDirectives[i], 0));
// 	for (std::vector<Token*>::iterator it = server.begin(); it != server.end(); ++it)
// 	{
// 		if (((*it)->type == DIRECTIVE))
// 		{
// 			std::map<std::string, int>::iterator	j = directivePrensent.find((*it)->value);
// 			if (j != directivePrensent.end())
// 				directivePrensent[(*it)->value]++;
// 		}
// 	}
// 	std::map<std::string, int>::iterator	k = directivePrensent.begin();
// 	for (; k != directivePrensent.end(); k++)
// 	{
// 		if ()
// 		if (directivePrensent["listen"] > 1 || directivePrensent[] >)
// 	}
	
// }

void	ConfigFile::analyseTokenList()
{	
	checkBrace();
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{
		if ((*it)->type == DIRECTIVE && (*it)->value == "server")
		{
			std::vector<Token*>	server;
			while (++it != _tokenList.end())
			{
				if ((*it)->type == DIRECTIVE && (*it)->value == "server")
					break;
				server.push_back( *it );
				// if ((*it)->value == "location" && (*it)->type == DIRECTIVE)
				// {
				// 	while ((*(++it))->type != BRACE_OP)
				// 		server.push_back( *it );
				// 	std::vector<Token*>	location;
				// 	while ((*(++it))->type != BRACE_CL)
				// 		location.push_back( *it );
				// 	// checkLocationContent( location );
				// 	for (std::vector<Token*>::iterator it = location.begin(); it != location.end(); ++it)
				// 		std::cout << "type: "<< (*it)->type << " | value: " << (*it)->value << '\n';
				// 	std::cout << "--------------------------------------------------------------------\n";
				// }
			}
			// checkServerContent( server );
			std::cout << "---------------------------------print server block-----------------------------------\n";
			for (std::vector<Token*>::iterator it = server.begin(); it != server.end(); ++it)
				std::cout << "type: "<< (*it)->type << " | value: " << (*it)->value << '\n';
			std::cout << "--------------------------------------------------------------------\n";
		}
		else
		{
			// std::cout << (*it)->value;
			throw std::invalid_argument( "invalid directive present out of server block" );
		}
	}
}