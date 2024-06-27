/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:39:38 by yachen            #+#    #+#             */
/*   Updated: 2024/06/27 17:25:43 by yachen           ###   ########.fr       */
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

void	print( std::vector<Token*>& tokenList )
{
	std::cout << "_______________________print tokenlist__________________________________\n";
	for (std::vector<Token*>::iterator it = tokenList.begin(); it != tokenList.end(); ++it)
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
	std::cout << "__________________________________________________________________________\n";
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
}

// verifie le {} sont correctement placees et fermees
void	ConfigFile::checkBrace()
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

// definie les regles de configuration pour les parametres des directives 
void	checkDirectiveParameter( std::vector<Token*>& directive )
{
	std::string	allowCaracters( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_" );
	size_t	size = directive.size();
	if (directive[0]->value == "listen" || directive[0]->value == "host" || directive[0]->value == "serverName"
		|| directive[0]->value == "clentMaxBodySize" || directive[0]->value == "autoindex")
	{
		if (size != 2)
			throw std::invalid_argument( directive[0]->value + ": invalid number of parameter" );
	}
	else if (directive[0]->value == "serverName" && directive[1]->value.find_first_not_of( allowCaracters ) != std::string::npos)
		throw std::invalid_argument( directive[0]->value + ": invalid name" );
	else if (directive[0]->value == "clientMaxBodySize" && directive[1]->value.find_first_not_of( "0123456789" ) != std::string::npos)
		throw std::invalid_argument( directive[0]->value + ": invalid size" );
	else if (directive[0]->value == "autoindex" && directive[1]->value != "on" && directive[1]->value == "off")
		throw std::invalid_argument( directive[0]->value + ": can't be other than on or off" );
	else if (directive[0]->value == "root")
	{
		if (size != 2 && size != 3)
			throw std::invalid_argument( directive[0]->value + ": invalid number of parameter" );
	}
	else if (directive[0]->value == "allowMethods")
	{
		if (size < 2 || size > 4)
			throw std::invalid_argument( directive[0]->value + ": invalid number of parameter" );
		for (size_t i = 1; i < size; i++)
		{
			if (directive[i]->value != "GET" && directive[i]->value != "POST" && directive[i]->value != "DELETE")
				throw std::invalid_argument( "unknow method: " + directive[i]->value );
		}
	}
	else if (directive[0]->value == "location" && size != 1)
		throw std::invalid_argument( directive[0]->value + ": location can't have parameter" );
	else if (directive[0]->value == "errorPages")
	{	
		for (size_t i = 1; i < size; i += 2)
		{
			if (directive[i]->value != "404" && directive[i]->value != "500")
				throw std::invalid_argument( directive[0]->value + ": unknown response code: " + directive[i]->value );
		}
	}
	else if (directive[0]->value == "return")
	{
		if (size != 3)
			throw std::invalid_argument( directive[0]->value + ": invalid number of parameter" );
		if (directive[1]->value != "301" && directive[1]->value != "302")
			throw std::invalid_argument( directive[0]->value + ": unkown response code" );		
	}
}

// verifie	la presence des directives obligatoires
// 			la presence des derectives inconnues ou doublees
// du block location
void	checkLocation( std::vector<Token*>& location )
{
	int	tab[4] = {0};	
			// std::cout << "_____________________directiveTab_location__________________\n";
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
			// std::cout << directive[0]->value << " | " << directive[1]->value << '\n';
			checkDirectiveParameter( directive );
			--i;
		}
	}
			// std::cout << "____________________________________________________\n";
	if (tab[0] != 1 || tab[1] > 1 || tab[2] > 1 || tab[3] > 1)
		throw std::invalid_argument( "directive error in location block" );
}

// verifie	la presence des directives obligatoires
// 			la presence des derectives inconnues ou doublees
// du block server
void	checkServer( std::vector<Token*> server )
{
	int tab[7] = {0};
				// std::cout << "_____________________directiveTab_server__________________\n";
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
				// std::cout << directive[0]->value << " | " << directive[1]->value << '\n';
				checkDirectiveParameter( directive );
			}
			else
				++i;
			--i;
		}
	}
				// std::cout << "____________________________________________________\n";
	if (tab[0] != 1 || tab[1] != 1 || tab[2] > 1 || tab[3] != 1 || tab[4] != 1 || tab[5] != 1
		|| tab[6] < 1)
		throw std::invalid_argument( "directive error in server block" );
}

void	ConfigFile::analyseTokenList()
{	
	checkBrace();
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{
		if ((*it)->type == DIRECTIVE && (*it)->value == "server")
		{
			std::vector<Token*>	server;
			++it;
			while (it != _tokenList.end())
			{
			 	if ((*it)->type == DIRECTIVE && (*it)->value == "server")
				{
					--it;
					break;
				}
				server.push_back( *it );
				if ((*it)->type == DIRECTIVE && (*it)->value == "location")
				{
					std::vector<Token*> location;
					++it;
					while (it != _tokenList.end())
					{
						location.push_back( *it );
						if ((*it)->type == BRACE_CL)
						{
							checkLocation( location );	// verifie les directives et les parmetres du block location.
							break;
						}
						++it;
					}
					print( location );
				}
				++it;
			}
			print( server );
			checkServer( server );	// verifie les directives et les parmetres du block server.
			if (it == _tokenList.end())
				break;
		}
		else
			throw std::invalid_argument( "invalid directive present out of server block" );
		
	}
}
