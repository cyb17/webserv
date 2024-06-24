/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joannpdetorres <joannpdetorres@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/24 21:55:34 by joannpdetor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TcpSocket.hpp"

TcpSocket::TcpSocket(std::string &IpAdress, std::string &port) : _adressIp(IpAdress), _port(port) 
{
	_serverSocket = -1;
	//_res = NULL;
	return ; 
}

TcpSocket::~TcpSocket() { return ; }

void    TcpSocket::init()
{
    // creer une socket pour le serveur
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
		handle_error("socket", 1);

	// lier l'adresse IP et le port au serveur
	/*addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(_adressIp.c_str(), _port.c_str() , &hints, &_res) != 0)
		handle_error("getaddrinfo", 0);
	if (bind(_serverSocket, _res->ai_addr, _res->ai_addrlen) == -1)
		handle_error("bind", 1);*/

	//ecouter via la socket pour détecter des demandes de connexion
	//if (listen(_serverSocket, BACKLOG) == -1)
		//handle_error("listen", 1);

	//ajouter le server socket a la liste de socket
	//pollfd serverFd = {_serverSocket, POLLIN, 0};
	//_listSockets.push_back(serverFd);
	close(_serverSocket);
}

/*void	TcpSocket::acceptNewConnexion()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1)
		handle_error("accept", 1);
	pollfd clientFd = {clientSocket, POLLIN, 0};
	_listSockets.push_back(clientFd);
}

void	TcpSocket::run()
{
	while (true)
	{
		if (poll(_listSockets.data(), _listSockets.size(), -1) == -1)
			handle_error("poll", 1);
		for (std::vector<struct pollfd>::iterator it = _listSockets.begin(); it != _listSockets.end(); ++it)
		{
			if (it->revents & POLL_IN)
			{
				if (it->fd == _serverSocket)
					acceptNewConnexion();
				else 
				{
					char buffer[1024];
					int valread = read(it->fd, buffer, 1024);
					if (valread > -1)
						std::cout << "Request received\n" << buffer << "\n";
					close(it->fd);
					it = _listSockets.erase(it);
					--it;
				}
			}	
		}	
	
	}
	close(_serverSocket);
}*/

void	TcpSocket::handle_error(const char *err, int i)
{
	/*if (!i)
		std::cerr << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else*/
	(void)i;
		std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
	if (_serverSocket > -1)
		close(_serverSocket);
	/*if (_res != NULL)
		freeaddrinfo(_res);*/
	exit(errno);
}