/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/04 18:24:22 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(std::string requestLine, Server& configServer) : _request(requestLine), _configServer( configServer ), _code( 0 )
{
	infos.bodyLenghtRequest = 0;
	// std::cout << _code << '\n';
}

Request::~Request() {}

void	Request::printInfos()
{
	std::cout << "REQUEST INFOS\n\n";
	std::cout << "* method = " << infos.method << "\n";
	std::cout << "* path = " << infos.path << "\n";
	std::cout << "* version = " << infos.version << "\n";
	std::cout << "* host = " << infos.host << "\n";
	for (unsigned long i = 0; i << infos.body.size(); ++i)
	{
		if (i == 0)
			std::cout << "* body = " << infos.body[i] << "\n";
		else
			std::cout << "         " << infos.body[i] << "\n";
	}
}

std::string	Request::buildResponse()
{
	parseRequest(_request);
	std::cout << "Code = " << getCode() << "\n\n";
	printInfos();
	return "hello world";
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
	if (infos.bodyLenghtRequest > 0)
	{
		int	len = 0;
		while (std::getline(request, line) && line != "\0")
		{
			len += line.size();
			std::string::iterator it = line.end() - 1;
			if (*it == '\n')
				len++;
			infos.body.push_back(line);
			if (len > infos.bodyLenghtRequest || len > 1024)
			{
				_code = 400;
				return;
			}
			else if (len == infos.bodyLenghtRequest)
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
	infos.method = lineInfo[0];
	infos.path = lineInfo[1];
	infos.version = lineInfo[2];
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
			infos.host = word;
		}
		else if (line[0] == "Content-Type:" && infos.method == "POST" )
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
			infos.bodyLenghtRequest = atoi(word.c_str());
		}
	}
	if (infos.host.empty())
		return (_code = 400, false);
	return (true);
}

int		Request::getCode()
{
	return (_code);
}

// Request::Request() : _socket(-1), _step(0), _line(""), _responseCode(0) {}
	
// Request::Request(int socket) : _socket(socket), _step(0),_line(""),  _responseCode(0) {}

// Request::~Request() {} 

// int Request::parsing()
// {
// 	char buffer[1];
// 	int len = 1;

// 	while (len > 0)
// 	{
// 		len = recv(_socket, buffer, 1, 0);
// 		if (len == -1)
// 		{
// 			if (errno != EAGAIN || errno != EWOULDBLOCK)
// 			{
// 				printMsgError("recv");
// 				return (_responseCode = 500);
// 			}
// 		}
// 		buffer[len] = '\0';
// 		_line += buffer;
// 		if (_step == 0 && _line != "\r\n")
// 				_step = 1;
// 		if (_step > 0 && buffer[0] == '\n') 
// 		{
// 			if (init() > 0)
// 				break;
// 			else
// 				_line = "";
// 		}
// 	}
// 	return (_responseCode);
// }

// int Request::init()
// {
// 	if (_firstLine.empty() && _line != "\r\n" && _step == 1)
// 		return (setFirstLine());
// 	if (_step == 2)
// 		return (setHeaders());
// 	// if (_step == 3)
// 	// {
// 	// 	if (_line == "\r")
// 	// 		return (_responseCode = 200);
// 	// 	else 
// 	// 	{
// 	// 		_body.push_back(_line);
// 	// 		return (_responseCode = 0);
// 	// 	}
// 	// }
// 	return (0);
// }

// //SETTERS

// int Request::setFirstLine()
// {
// 	std::istringstream iss(_line);
// 	std::string	word;
// 	std::string firstline[3] = {"method", "url", "http"};
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		if (iss >> word)
// 			_firstLine.insert(std::make_pair(firstline[i], word));
// 	}
// 	if (iss >> word || _firstLine.size() != 3)
// 		return (_responseCode = 400);
// 	if (checkFirstLine() == false)
// 		return (_responseCode);
// 	_step++;
// 	return (0);
// }

// int Request::setHeaders()
// {
// 	if (_line == "\r\n")
// 	{
// 		if (_headers.empty())
// 			return(_responseCode = 400);
// 		else
// 			return (_step++, _responseCode = 200);
// 	}
// 	std::istringstream iss(_line);
// 	std::string header;
// 	std::string word;
// 	std::string value;
		
// 	iss >> header;
// 	while (iss >> word)
// 	{
// 		value += word;
// 		value += " ";
// 	}
// 	if (header == "Host:" && value.empty())
// 		return (_responseCode = 400);
// 	_headers.insert(std::make_pair(header, value));
// 	return (0);
// }

// // CHECK FUNCTIONS

// bool Request::checkFirstLine()
// {
// 	std::map<std::string,std::string>::iterator it;
	
// 	//Check method
// 	it = _firstLine.find("method");
// 	std::string method[3] = {"GET", "POST", "DELETE"};
// 	if (it != _firstLine.end())
// 	{
// 		int i = 0;
// 		while (i < 3)
// 		{
// 			if (it->second == method[i++])
// 				break;
// 		}
// 		if (i == 3)
// 			return (_responseCode = 400, false);
// 	}
// 	//Check version http
// 	it = _firstLine.find("http");
// 	if (it->second != "HTTP/1.1")
// 		return (_responseCode = 400, false);
// 	//Check url
// 	it = _firstLine.find("url");
// 	if (it != _firstLine.end())
// 	{
// 		std::string path = it->second;
// 		std::ifstream file(path.c_str());
		
// 		if (!file)
// 			return (_responseCode = 404, false);
// 		file.close();
// 	}
// 	return (true);
// }

// bool Request::checkHeaders()
// {
// 	std::map<std::string,std::string>::iterator it;
	
//     for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
// 	{
//     	if (it->first == "Host:")
// 			return (true);
// 	}
// 	return (false);
// }

// //GETTERS

// int	Request::getResponseCode() { return (_responseCode); }

// //PRINT FUNCTIONS

// void Request::printFirstLine() const
// {
//     std::map<std::string, std::string>::const_iterator it;
	
//     for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
//         std::cout << it->first << ": " << it->second << "\n";
// }

// void Request::printHeaders() const
// {
//     std::map<std::string, std::string>::const_iterator it;
	
//     for (it = _headers.begin(); it != _headers.end(); ++it)
//         std::cout << it->first << " " << it->second << "\n";
// }

// void Request::printMsgError(const char *err)
// {
// 	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
// }