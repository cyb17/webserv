/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joannpdetorres <joannpdetorres@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/24 16:03:45 by joannpdetor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TcpSocket.hpp"

TcpSocket::TcpSocket() { return ; }

TcpSocket::TcpSocket(std::string IpAdress, int port) : _IpAdress(IpAdress), _port(port) { return ; }

TcpSocket::~TcpSocket() { return ; }

void    TcpSocket::init()
{
   
}

void TcpSocket::handle_error(char *err, int fd, int i)
{
	if (!i)
		std::cout << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cout << "ERROR : " << err << " => " << strerror(errno) << "\n";
	if (fd > -1)
		close(fd);
	exit(errno);
}