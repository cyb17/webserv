/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/04 14:26:09 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( char str[], size_t size, Server& configServer) : _request( str, size ), _configServer( configServer ), _code( 200 )
{
	std::cout << _code << '\n';
}

Request::~Request() {}

std::string	Request::buildResponse()
{
	ReponseInfo	info;
	
	parseRequest( info );
	// check _code.
	return "hello world";
}

void	Request::parseRequest( Reponse& info)
{
	std::istringstream	request( _request );
	std::string	requestLine;
	if (std::getline( request, requestLine ))
	{
		if (isGoodRequestLine( requestLine, info ) == false)
			return;
	}
	std::vector<std::string>	headers;
	std::string	line;
	while (std::getline( request, line ) && !line.empty())
		headers.push_back( line );
	if (isGoodHeaders( headers, info ) == false)
		return;
	while (std::getline( request, line ))
		info.body.push_back( line );
	if (isGoodBody( info.body, info ) == false)
		return;
}

int	Request::isGoodRequestLine( std::string& requestLine, ReponseInfo& info )
{
	std::istringstream	iss( requestLine );
	std::vector<std::string>	lineInfo;
	std::string word;
	while ( iss >> word )
		lineInfo.push_back( word );
	if (lineInfo.size() != 3 || requestLine.find( "\r\n", 0 ) == std::string::npos 
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE")
		|| lineInfo[2] != "HTTP/1.1")
	{
		_code = 400;
		return false;
	}
	info.method = lineInfo[0];
	info.path = lineInfo[1];
	info.version = lineINfo[2];
	return true;
}

int	Request::isGoodHeaders( std::vector<std::string>& headers )
{
	return true;
}

int	Request::isGoodBody( std::vector<string>& body, ReponseInfo& info )
{
	
}
