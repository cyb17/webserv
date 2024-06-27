/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/28 00:14:04 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { return ; }
	
Request::Request(int socket) : _socket(socket)
{
	_start = 0;
	_responseCode = 0;
	_line = "";
	return ;
}

Request::~Request() { return ; }

int Request::init()
{
	char buffer[1];
	ssize_t len;

	while (len > 0)
	{
		len = recv(_socket, buffer, 1, 0);
		if (len == -1)
		{
			if (errno == EAGAIN && errno == EWOULDBLOCK)
				return (_responseCode = 0);
			diplayMsgError("recv");
			return (_responseCode = 500);
		}
		if (buffer == "\n") 
		{
			if (setStructRequest(buffer) > 0)
				break ;
			else
				_line == "";
		}
		else
		{
			if (_start == 0)
				_start = 1;
			_line += buffer;
		}
	}
	return (_responseCode = 200);
}

int Request::setStructRequest(char *buffer)
{
	if (_start >= 1)
	{
		if (_firstLine.empty() && _start == 1)
		{
			std::istringstream iss(_line);
			std::string	word;
			for (int i = 0; i < 3; ++i)
			{
				if (iss >> word)
					_firstLine.insert(std::make_pair(firstline[i], word));
			}
			if (iss >> word || _firstLine.size() != 3)
				return (_responseCode = 400);
			_start++;
			return (_responseCode = 0);
		}
		if (_headers.empty())
	}
}


void Request::diplayMsgError(const char *err)
{
	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}