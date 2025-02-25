/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/23 16:48:01 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {}

Request::Request( Server& configServer, Server& defaultServer ) : _defaultConfigServer(defaultServer), _configServer( configServer ), _code( 200 ), _step( firstLine )
{
	time( &_startTime );
	_infos.bodyLengthRequest = -1;
	_infos.bodyLen = 0;
	_endOfFullRequest = false;
}

Request::~Request() {}

// check request line, headers, body length and body.
Step	Request::parseRequest( std::string& requestLine )
{
	std::istringstream	request(requestLine);
	std::string line;
	
	if (_step == firstLine && std::getline(request, line))
	{
		if (line == "\r")
			return (_step);
		if (isGoodRequestLine( line ) == false)
			return (_step = complete);
		_step = headers;
	}
	if (_step == headers)
	{
		while (std::getline(request,line) && line != "\r")
			_headersTmp.push_back(line);
		if (_headersTmp.empty() && line == "\r")
			return (_code = 400, _step = complete);
		if (line == "\r")
		{
			_step = body;
			if (isGoodHeaders(_headersTmp) == false || _infos.method != "POST")
				return (_step = complete);
		}
	}
	if (_step == body)
	{
		if (_infos.bodyLengthRequest <= 0 || _infos.bodyLengthRequest > _configServer.clientMaxBodySize)
		{
			std::cerr << "Error: request body length\n";
			return (_code = 400, _step = complete);
		}
		while (std::getline(request, line))
		{
			_infos.bodyLen += line.size();
			if (_infos.body.empty() && line == "\r")
				return (_code = 400, _step = complete);
			if (_infos.contentType == "multipart/form-data" && request.eof() == false)
			{
				_infos.bodyLen++;
				line += "\n";
			}
			_infos.body.push_back(line);
		}
		if (_infos.bodyLen == _infos.bodyLengthRequest)
			return (addInfos(), _step = complete);
		if (_infos.bodyLen > _infos.bodyLengthRequest)
			return _code = 400, _step = complete;
		if (_endOfFullRequest == true && _infos.bodyLen < _infos.bodyLengthRequest)
			return _code = 400, _step = complete;
	}
	return (_step);
}

//	remplie ReponseInfos fileName et fileBody
void	Request::addInfos()
{
	if (_infos.contentType == "application/x-www-form-urlencoded")
	{
		for (size_t i = 0; i < _infos.body.size(); ++i)
			_infos.fileBody += _infos.body[i];
	}
	else if (_infos.contentType == "multipart/form-data")
	{
		size_t i = 0;
		while (i < _infos.body.size() && _infos.body[i] != "\r\n")
		{	
			if (_infos.fileName.empty())
			{
				size_t tmp = _infos.body[i].find("filename=");
				if (tmp != std::string::npos)
				{
					_infos.fileName = _infos.body[i].substr(tmp + 10);
					_infos.fileName.erase(_infos.fileName.size() - 3);
				}
			}
			i++;
		}
		while (++i < _infos.body.size() - 1)
			_infos.fileBody += _infos.body[i];
	}
}

// remplie ResponseInfos : method, locationRoot, locationFile, queryString(pour GET)
bool	Request::isGoodRequestLine( std::string& requestLine)
{
	std::istringstream	iss( requestLine );
	std::vector<std::string>	lineInfo;
	std::string word;
	
	while ( iss >> word )
		lineInfo.push_back( word );
	if (lineInfo.size() != 3 || lineInfo[2] != "HTTP/1.1"
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE"))
		return _code = 400, false;
		
	_infos.method = lineInfo[0];
	std::size_t	lastSlash = lineInfo[1].find_last_of( '/', lineInfo[1].size() );
	_infos.locationRoot = lineInfo[1].substr( 0, lastSlash + 1 );
	_infos.locationFile = lineInfo[1].substr( lastSlash + 1, lineInfo[1].length() );
	
	if (_infos.method == "GET")
	{
		std::size_t		interrogation = _infos.locationFile.find( '?');
		if (interrogation != std::string::npos)
		{
			_infos.queryString = _infos.locationFile.substr(interrogation + 1, _infos.locationFile.size());
			_infos.locationFile = _infos.locationFile.substr(0, interrogation);
		}
	}
	return true;
}

// remplie : Host, Content-type
bool	Request::isGoodHeaders( std::vector<std::string>& headers )
{
	for (unsigned long i = 0; i < headers.size(); ++i)
	{
		std::istringstream iss(headers[i]);
		std::vector<std::string> line;
		std::string word;
		while (iss >> word)
			line.push_back(word);
		if (line.empty())	// pour eviter des pb de segfault.
			continue;
		if (line[0] == "Host:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			_infos.host = word;
		}
		else if (line[0] == "Content-Type:" && _infos.method == "POST" )
		{
			if ( line[1] == "application/x-www-form-urlencoded")
				_infos.contentType = line[1];
			else if (line[1].find("multipart/form-data") != std::string::npos)
				_infos.contentType = "multipart/form-data";
		}
		else if (line[0] == "Content-Length:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			for (unsigned long i = 0; i < line[1].size(); ++i)
			{
				if (!isdigit(line[1][i]) || (line[1].size() == 1 && word[0] == '0'))
					return (_code = 400, false);
			}
			_infos.contentLength = line[1];
			_infos.bodyLengthRequest = atoi(word.c_str());
		}
	}
	if (_infos.host.empty() || (_infos.contentType.empty() && _infos.bodyLengthRequest > 0 ))
		return (_code = 400, false);
	return (true);
}

void			Request::setEndOfFullRequest() { _endOfFullRequest = true; }

int				Request::getCode() { return (_code); }
time_t			Request::getStartTime() { return (_startTime); }
bool			Request::getEndOfFullRequest() { return (_endOfFullRequest); }
ResponseInfos	Request::getResponseInfos() { return (_infos); }
Server 			Request::getDefaultConfig() { return (_defaultConfigServer); }
Server			Request::getServerConfig()  { return (_configServer); }
