/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/06 15:06:12 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string requestLine, Server& configServer) : _request(requestLine), _configServer( configServer ), _code( 200 )
{
	_infos.bodyLenghtRequest = 0;
}

Request::~Request() {}

void	Request::printInfos()
{
	std::cout << "Code = " << _code << "\n\n";
	std::cout << "REQUEST INFOS\n\n";
	std::cout << "* method = " << _infos.method << "\n";
	std::cout << "* path = " << _infos.path << "\n";
	std::cout << "* version = " << _infos.version << "\n";
	std::cout << "* host = " << _infos.host << "\n";
	if (_infos.body.size() > 0)
	for (unsigned long i = 0; i << _infos.body.size(); ++i)
	{
		if (i == 0)
			std::cout << "* body = " << _infos.body[i] << "\n";
		else
			std::cout << "         " << _infos.body[i] << "\n";
	}
}

std::string getGMTDate()
{
	time_t		currentTime;
	struct tm*	timeInfos;
	const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time( &currentTime );
	timeInfos = gmtime( &currentTime );
	std::stringstream	ss;
	
	ss << "Date: "
       << days[timeInfos->tm_wday] << ", "
       << std::setfill('0') << std::setw(2) << timeInfos->tm_mday << " "
       << months[timeInfos->tm_mon] << " "
       << (timeInfos->tm_year + 1900) << " "
       << std::setfill('0') << std::setw(2) << timeInfos->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << timeInfos->tm_min << ":"
       << std::setfill('0') << std::setw(2) << timeInfos->tm_sec
       << " GMT";
	return ss.str();
}

std::string	Request::buildResponse()
{
	parseRequest(_request);
	printInfos();
	std::string response;
	std::string date = getGMTDate();
	std::string server = _configServer.serverName + "\r\n";
	// if (_code == 400)
	// {
		response = "HTTP/1.1 400 Bad Request\r\n" + date + "\r\n" + server + "\r\n" + "Content-Type: text/plain\r\n" + "Content-Length: 13\r\n\r\n" + "body...";
	// }
	return response;
}

void	Request::parseRequest( std::string& requestLine)
{
	std::istringstream	request(requestLine);
	std::string line;
	
	if (std::getline(request, line))
	{
		std::cout << "first line: " << line << "\n";
		if (isGoodRequestLine( line ) == false)
			return;
	}
	std::vector<std::string>	headers;
	while (std::getline(request,line) && line != "\r")
		headers.push_back(line);
	if (isGoodHeaders(headers) == false)
		return;
	if (_infos.bodyLenghtRequest > 0)
	{
		int	len = 0;
		while (std::getline(request, line) && line != "\0")
		{
			len += line.size();
			std::string::iterator it = line.end() - 1;
			if (*it == '\n')
				len++;
			_infos.body.push_back(line);
			if (len > _infos.bodyLenghtRequest || len > 1024)
			{
				_code = 400;
				return;
			}
			else if (len == _infos.bodyLenghtRequest)
				break ;
		}
	}
}

bool	Request::isGoodRequestLine( std::string& requestLine)
{
	std::istringstream	iss( requestLine );
	std::vector<std::string>	lineInfo;
	std::string word;
	while ( iss >> word )
		lineInfo.push_back( word );
	if (lineInfo.size() != 3 /*|| requestLine.find( "\r\n", 0 ) == std::string::npos*/
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE")
		|| lineInfo[2] != "HTTP/1.1")
	{
		_code = 400;
		std::cout << "code error\n";
		return false;
	}
	_infos.method = lineInfo[0];
	_infos.path = lineInfo[1];
	_infos.version = lineInfo[2];
	return true;
}

bool	Request::isGoodHeaders( std::vector<std::string>& headers )
{
	if (headers.empty())
		return (_code = 400, false);
	int contentType = 0;
	for (unsigned long i = 0; i < headers.size(); ++i)
	{
		std::istringstream iss(headers[i]);
		std::vector<std::string> line;
		std::string word;
		while (iss >> word)
			line.push_back(word);
		if (line[0] == "Host:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			_infos.host = word;
		}
		else if (line[0] == "Content-Type:" && _infos.method == "POST" )
			contentType = 1;
		else if (contentType && line[0] == "Content-Lenght:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			for (unsigned long i = 0; i < line[1].size(); ++i)
			{
				if (!isdigit(word[i]) || (line[1].size() == 1 && word[0] == '0'))
					return (_code = 400, false);
			}
			_infos.bodyLenghtRequest = atoi(word.c_str());
		}
	}
	if (_infos.host.empty())
		return (_code = 400, false);
	return (true);
}

// int		Request::getCode()
// {
// 	return (_code);
// }