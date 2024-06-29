/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/29 15:54:38 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _socket(-1), _step(0),_line(""), _responseCode(0) {}
	
Request::Request(int socket) : _socket(socket), _step(0),_line(""),  _responseCode(0) {}

Request::~Request() {} 

int Request::parsing()
{
	char buffer[1];
	int len = 1;

	while (len > 0)
	{
		len = recv(_socket, buffer, 1, 0);
		if (len == -1)
		{
			if (errno != EAGAIN || errno != EWOULDBLOCK)
			{
				printMsgError("recv");
				return (_responseCode = 500);
			}
		}
		buffer[len] = '\0';
		_line += buffer;
		if (_step == 0 && _line != "\r\n")
				_step = 1;
		if (_step > 0 && buffer[0] == '\n') 
		{
			if (init() > 0)
				break;
			else
				_line = "";
		}
	}
	return (_responseCode);
}

int Request::init()
{
	if (_firstLine.empty() && _step == 1)
		return (setFirstLine());
	if (_step == 2)
		return (setHeaders());
	// if (_step == 3)
	// {
	// 	if (_line == "\r")
	// 		return (_responseCode = 200);
	// 	else 
	// 	{
	// 		_body.push_back(_line);
	// 		return (_responseCode = 0);
	// 	}
	// }
	return (0);
}

//SETTERS

int Request::setFirstLine()
{
	std::istringstream iss(_line);
	std::string	word;
	std::string firstline[3] = {"method", "url", "http"};
	for (int i = 0; i < 3; ++i)
	{
		if (iss >> word)
			_firstLine.insert(std::make_pair(firstline[i], word));
	}
	if (iss >> word || _firstLine.size() != 3)
		return (_responseCode = 400);
	if (checkFirstLine() == false)
		return (_responseCode);
	_step++;
	return (0);
}

int Request::setHeaders()
{
	if (_line == "\r\n")
	{
		if (_headers.empty())
			return(_responseCode = 400);
		else
			return (_step++, _responseCode = 200);
	}
	std::istringstream iss(_line);
	std::string header;
	std::string word;
	std::string value;
		
	iss >> header;
	while (iss >> word)
	{
		value += word;
		value += " ";
	}
	if (header == "Host:" && value.empty())
		return (_responseCode = 400);
	_headers.insert(std::make_pair(header, value));
	return (0);
}

// CHECK FUNCTIONS

bool Request::checkFirstLine()
{
	std::map<std::string,std::string>::iterator it;
	
	//Check method
	it = _firstLine.find("method");
	std::string method[3] = {"GET", "POST", "DELETE"};
	if (it != _firstLine.end())
	{
		int i = 0;
		while (i < 3)
		{
			if (it->second == method[i++])
				break;
		}
		if (i == 3)
			return (_responseCode = 400, false);
	}
	//Check url
	it = _firstLine.find("url");
	if (it != _firstLine.end())
	{
		std::string path = "ressources" + it->second;
		std::ifstream file(path.c_str());
		
		if (!file)
			return (_responseCode = 404, false);
		file.close();
	}
	//Check version http
	it = _firstLine.find("http");
	if (it->second != "HTTP/1.1")
		return (_responseCode = 400, false);
	return (true);
}

bool Request::checkHeaders()
{
	std::string headers[3] = {"Host:", "User-Agent:", "Connection:", "Content-Type", "Content-Lenght"};
	std::map<std::string,std::string>::iterator it;
	
    for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
	{
        int i = 0;
		while (i < 3)
		{
			if (it->first == headers[i++])
				break;
		}
		if (i == 3)
			return (false);
	}
	
	return (true);
}

//GETTERS

int	Request::getResponseCode() { return (_responseCode); }


//PRINT FUNCTIONS

void Request::printFirstLine() const
{
    std::map<std::string, std::string>::const_iterator it;
	
    for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
        std::cout << it->first << ": " << it->second << "\n";
}

void Request::printHeaders() const
{
    std::map<std::string, std::string>::const_iterator it;
	
    for (it = _headers.begin(); it != _headers.end(); ++it)
        std::cout << it->first << " " << it->second << "\n";
}

void Request::printMsgError(const char *err)
{
	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}