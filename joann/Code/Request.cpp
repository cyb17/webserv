/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/27 12:14:40 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { return ; }
	
Request::Request(int socket) : _socket(socket)
{
	_valid == true;
	return ;
}

Request::~Request() { return ; }

Connexion Request::init()
{
	if (_firstLine.empty() && !setFirstline())
		return (CONNECT);
	
}

Connexion Request::setFirstline()
{
	char buffer[1];
	std::string line = "";
	ssize_t len;
	int	start;

	while (len > 0)
	{
		len = recv(_socket, buffer, 1, 0);
		if (len == -1)
		{
			if ((errno == EAGAIN && errno == EWOULDBLOCK) && start == 0)
				return (CONNECT);
			diplayMsgError("recv");
			_valid = false;
			return (DISCONNECT);
		}
		if (buffer == "\n") 
		{
			if (line != "")
				break;
		}
		else
			line += buffer;
	}
	return (DISCONNECT);	
}


void Request::diplayMsgError(const char *err)
{
	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}