/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:39:38 by yachen            #+#    #+#             */
/*   Updated: 2024/06/25 17:58:28 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

ConfigFile::ConfigFile( char* path ) : _path( path ) {}

ConfigFile::~ConfigFile() {}

Token*	makeToken( std::string& value )
{
	std::string	directives[12] = { "server", "listen", "host", "serverName", "clientMaxBodySize",\
	 "root", "index", "errorPages", "location", "allowMethods", "autoindex", "return" };
	Token* token = new Token;
	for (int i = 0; i < 12; i++)
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
	else if (value == ";")
		token->type = SEMICOLON;
	else
		token->type = PARAMETER;
	token->value = value;
	return token;
}

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
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
		std::cout << "type: "<< (*it)->type << " | value: " << (*it)->value << '\n';
}

void	ConfigFile::analyseTokenList()
{	
	std::vector<Token*>	server
	for (std::vector<Token*>::iterator it = _tokenList.begin(); it != _tokenList.end(); ++it)
	{
		if ((*it)->type == DIRECTIVE && (*it + 1)->type == BRACE_OP)
		{
				
		}
	}
}