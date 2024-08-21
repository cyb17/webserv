/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:39:38 by yachen            #+#    #+#             */
/*   Updated: 2024/07/15 16:15:06 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

ConfigParser::ConfigParser( char* path ) : _path( path ) {}

ConfigParser::~ConfigParser()
{
	for (size_t i = 0; i < _tokenList.size(); ++i)
		delete _tokenList[i];
	_tokenList.clear();

}
/*************************************************************************************************/
//								PUBLIC FONCTIONS
/*************************************************************************************************/

// verifie l'ouverture et la lecture du fichier de config, construire une liste de Tokens du contenu.
void	ConfigParser::fillTokenList()
{
	std::string		line;
	std::fstream	fd( _path, std::fstream::in );
	std::fstream	copy( _path, std::fstream::in );
	if (!fd.is_open())
		throw std::invalid_argument( "open configuration file failed" );
	std::getline( copy, line );
	if (copy.fail() || copy.eof())
		throw std::invalid_argument( "read data failed" );
	line.clear();
	while (std::getline(fd, line))
	{
		if (fd.fail())
			throw std::invalid_argument( "read data failed" );
		std::istringstream	ss( line );
		std::string	str;
		while ( ss >> str )
			_tokenList.push_back( makeToken( str ));
	}
	fd.close();
}

void	ConfigParser::analyseTokenList()
{	
	checkBrace();	// verifie si les accolades sont bien fermees et placees correctement. 
	checkNumberOfParameter();	// verifie le nb de parametres que contient chaque directives sont corrects.
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{
		if ((*it)->type == DIRECTIVE && (*it)->value == "server")	// entre dans le block server
		{
			++it;
			std::vector<Token*>	server;
			while (it != _tokenList.end())
			{
			 	if ((*it)->type == DIRECTIVE && (*it)->value == "server")
				{
					--it;  // ajuste la position de l'iterator pour lire le prochain server block
					break;
				}
				server.push_back( *it );
				if ((*it)->type == DIRECTIVE && (*it)->value == "location")	// entre dans le block location.
				{
					++it;
					std::vector<Token*> location;
					while (it != _tokenList.end())
					{
						location.push_back( *it );
						if ((*it)->type == BRACE_CL)
						{
							checkLocation( location );	// verifie les directive et les parametres du block location.
							break;
						}
						++it;
					}
				}
				++it;
			}
			checkServer( server );	// verifie le reste des directives et parametres hors location.
			if (it == _tokenList.end())
				break;
		}
		else
			throw std::invalid_argument( "invalid directive present out of server block" );
	}
	checkHostPort();	// verifie s'il n'y a pas plusieurs serveurs avec un host:port identique.  
}

std::vector<Token*>&	ConfigParser::getTokenList()
{
	return _tokenList;
}

void	ConfigParser::printTokenList()
{
	for (size_t i = 0; i < _tokenList.size(); ++i)
	{
		if (_tokenList[i]->type == BRACE_CL)
			std::cout << "BRACE_CL";
		if (_tokenList[i]->type == BRACE_OP)
			std::cout << "BRACE_OP";
		if (_tokenList[i]->type == DIRECTIVE)
			std::cout << "DIRECTIVE";
		if (_tokenList[i]->type == PARAMETER)
			std::cout << "PARAMETER";
		std::cout << " :	 " << _tokenList[i]->value << '\n';
	}
}


/*************************************************************************************************/
//								PRIVATE FONCTIONS
/*************************************************************************************************/

Token*	ConfigParser::makeToken( std::string& value )
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

void	ConfigParser::checkBrace()
{
	int	openBrace = 0;
	int	closeBrace = 0;
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{	
		if ((*it)->type == BRACE_OP)
		{
			if (it  == _tokenList.begin() || (*(it - 1))->type != DIRECTIVE
				 || ((*(it - 1))-> type == DIRECTIVE
					&& (*(it - 1))->value != "server"
					&& (*(it - 1))->value != "location"
					&& (*(it - 1))->value != "errorPages"))
						throw std::invalid_argument( "bad information after a '{' detected" );
			openBrace++;
		}
		if ((*it)->type == BRACE_CL)
		{
			if (it + 1 != _tokenList.end() && (*(it + 1))-> type != DIRECTIVE && (*(it + 1))->type != BRACE_CL)
						throw std::invalid_argument( "bad information after a '}' detected" );
			closeBrace++;
		}
	}
	if (openBrace != closeBrace)
		throw std::invalid_argument( "brace not closed" );
}


void	ConfigParser::checkNumberOfParameter()
{
	for (size_t i = 0; i < _tokenList.size(); i++)
	{
		if (_tokenList[i]->type == DIRECTIVE)
		{
			std::string	value = _tokenList[i]->value;
			int	paramCount = 0;
			size_t	j = i + 1;
			while (j < _tokenList.size() && _tokenList[j]->type != DIRECTIVE)
			{
				if (_tokenList[j]->type == PARAMETER)
					paramCount++;
				j++;
			}
			if (((value == "server" || value == "location") && paramCount != 0)
				||	(value == "root" && (paramCount < 1 || paramCount > 2))
				||	(value == "allowMethods" && (paramCount < 1 || paramCount > 3))
				||	((value == "errorPages" || value == "return") && paramCount < 2)
				||	((value == "listen" || value == "host" || value == "serverName"
					|| value == "clientMaxBodySize" || value == "autoindex") && paramCount != 1))
				throw std::invalid_argument( value + ": invalide number of parameters" );
			i = j - 1;
		}
	}	
}

void	ConfigParser::checkHostPort()
{
	std::vector<std::pair<std::string, std::string> >	hostPort;
	size_t	size = _tokenList.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_tokenList[i]->type == DIRECTIVE && _tokenList[i]->value == "server")
		{
			std::string	host, port;
			while (++i < size && !(_tokenList[i]->type == DIRECTIVE && _tokenList[i]->value == "server"))
			{
				if (_tokenList[i]->type == DIRECTIVE && _tokenList[i]->value == "host")
					host = _tokenList[++i]->value;
				else if (_tokenList[i]->type == DIRECTIVE && _tokenList[i]->value == "listen")
					port = _tokenList[++i]->value;
			}
			hostPort.push_back( std::make_pair( host, port ) );
			if (i < size && _tokenList[i]->type == DIRECTIVE && _tokenList[i]->value == "server")
				--i;
		}
	}
	for (size_t i = 0; i < hostPort.size(); ++i)
	{
		for (size_t j = i + 1; j < hostPort.size(); ++j)
		{
			if (hostPort[i].first == hostPort[j].first && hostPort[i].second == hostPort[j].second)
				throw std::invalid_argument( "multiple servers have same listen value and host value" );
		}
	}
}

// definie les regles de configuration pour les parametres des directives 
void	ConfigParser::checkParameterContent( std::vector<Token*>& directive )
{
	std::string	allowCaracters( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ://._0123456789" );
	size_t	size = directive.size();
	
	if (directive[0]->value == "serverName" && directive[1]->value.find_first_not_of( allowCaracters ) != std::string::npos)
		throw std::invalid_argument( directive[0]->value + ": invalid name" );
	else if (directive[0]->value == "clientMaxBodySize")
	{
		if (directive[1]->value.find_first_not_of( "0123456789" ) != std::string::npos
			|| directive[1]->value.length() > 7 )
			throw std::invalid_argument( directive[0]->value + ": invalid size" );
	}	
	else if (directive[0]->value == "autoindex" && directive[1]->value != "on" && directive[1]->value != "off")
		throw std::invalid_argument( directive[0]->value + ": parameter can't be other than on or off" );
	else if (directive[0]->value == "allowMethods")
	{
		for (size_t i = 1; i < size; i++)
		{
			if (directive[i]->value != "GET" && directive[i]->value != "POST" && directive[i]->value != "DELETE")
				throw std::invalid_argument( "unknow method: " + directive[i]->value );
		}
	}
	else if (directive[0]->value == "return")
	{
		if (directive[1]->value != "301" && directive[1]->value != "302")
			throw std::invalid_argument( directive[0]->value + ": unkown response code" );		
	}
}

void	ConfigParser::checkLocation( std::vector<Token*>& location )
{
	int	tab[4] = {0};	
	for (size_t i = 0; i < location.size(); i++)
	{
		if (location[i]->type == BRACE_OP || location[i]->type == BRACE_CL)
			continue;
		else if (location[i]->type == DIRECTIVE)
		{
			if (location[i]->value == "root")
				tab[0]++;
			else if (location[i]->value == "allowMethods")
				tab[1]++;
			else if (location[i]->value == "autoindex")
				tab[2]++;
			else if (location[i]->value == "return")
				tab[3]++;
			std::vector<Token*> directive;
			directive.push_back( location[i++] );
			while (location[i]->type == PARAMETER)
				directive.push_back( location[i++] );
			checkParameterContent( directive );	// verifie le contenu de chaque directives.
			--i;
		}
	}
	if (tab[0] != 1 || tab[1] > 1 || tab[2] > 1 || tab[3] > 1)	// verifie les directives : obligatoires, non obligatoires, doublees.
		throw std::invalid_argument( "directive error in location block" );
}

void	ConfigParser::checkServer( std::vector<Token*> server )
{
	int tab[7] = {0};
	for (size_t i = 0; i < server.size(); i++)
	{
		if (server[i]->type == BRACE_OP || server[i]->type == BRACE_CL)
			continue;
		else if (server[i]->type == DIRECTIVE)
		{
			if (server[i]->value == "listen")
				tab[0]++;
			else if (server[i]->value == "host")
				tab[1]++;
			else if (server[i]->value == "serverName")
				tab[2]++;
			else if (server[i]->value == "clientMaxBodySize")
				tab[3]++;
			else if (server[i]->value == "root")
				tab[4]++;
			else if (server[i]->value == "errorPages")
				tab[5]++;
			else if (server[i]->value == "location")
				tab[6]++;
			if (server[i]->value != "location")
			{
				std::vector<Token*> directive;
				directive.push_back( server[i++] );
				if (server[i]->type == BRACE_OP)
					i++;
				while (server[i]->type == PARAMETER)
					directive.push_back( server[i++] );
				checkParameterContent( directive );
			}
			else
				++i;
			--i;
		}
	}
	if (tab[0] != 1 || tab[1] != 1 || tab[2] > 1 || tab[3] != 1 || tab[4] != 1 || tab[5] != 1
		|| tab[6] < 1)
		throw std::invalid_argument( "directive error in server block" );
}
