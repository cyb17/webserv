/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/28 19:12:42 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : _socket(-1), _step(0),_line(""), _responseCode(0) {}
	
Request::Request(int socket) : _socket(socket), _step(0),_line(""),  _responseCode(0) {}

Request::~Request() {} 

int Request::init()
{
	char buffer[1];
	int len = 1;

	while (len > 0)
	{
		len = recv(_socket, buffer, 1, 0);
		if (len == -1)
		{
			if (errno == EAGAIN && errno == EWOULDBLOCK)
				return (0);
			diplayMsgError("recv");
			return (_responseCode = 500);
		}
		buffer[len] = '\0';
		if (buffer[0] == '\n') 
		{
			if (setStructRequest(buffer) > 0)
				break;
			else
				_line = "";
		}
		else
		{
			if (_step == 0 && buffer[0] != '\r')
				_step = 1;
			if (_step > 0)
				_line += buffer;
		}
	}

	return (_responseCode);
}

int Request::setStructRequest(char *buffer)
{
	(void) buffer;
	if (_step >= 1)
	{
		if (_firstLine.empty() && _step == 1)
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
			_step++;
			return (0);
		}
		if (_step == 2)
		{
			if (_line == "\r")
			{
				if (_headers.empty())
					return(_responseCode = 400, 400);
				else
					return (_step++, 0);
			}
			else
			{
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
				_headers.insert(std::make_pair(header, value));
				return (0);
			}
		}
		if (_step == 3)
		{
			if (_line == "\r")
				return (_responseCode = 200);
			else 
			{
				_body.push_back(_line);
				return (_responseCode = 0);
			}
		}
	}
	return (0);
}

int	Request::getResponseCode()
{
	return (_responseCode);
}

void Request::printFirstLine() const
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

void Request::printHeadears() const
{
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it)
    {
        std::cout << it->first << it->second << std::endl;
    }
}

void Request::diplayMsgError(const char *err)
{
	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}