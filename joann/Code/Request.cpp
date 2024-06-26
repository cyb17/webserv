/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/26 16:45:29 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() { return ; }
	
Request::Request(int socket) : _socket(socket), _valid(true)
{
	return ;
}

Request::~Request() { return ; }

void Request::setMsg()
{
	char buffer[1];
	std::string line = "";
	ssize_t len;
	
	len = 1;
	while (len > 0)
	{
		len = recv(_socket, buffer, 1, 0);
		if (buffer == "\n" || buffer == NULL) 
			_msg.push_back(line);
		else
			line += buffer;
	}
}