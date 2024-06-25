/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/25 11:06:51 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(std::string &IpAdress, std::string &port) : _adressIp(IpAdress), _port(port) 
{
	_serverSocket = -1;
	return ; 
}

HttpServer::~HttpServer() 
{
	if (_serverSocket != -1)
		close(_serverSocket);
	if (_res)
		freeaddrinfo(_res);
	return ; 
}

void    HttpServer::init()
{
	//recuperer l'adresse IP et le port
	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(_adressIp.c_str(), _port.c_str() , &hints, &_res) != 0)
		handle_error("getaddrinfo", 0);
	
    // creer une socket pour le serveur
    _serverSocket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
    if (_serverSocket == 0)
		handle_error("socket", 1);

	// lier l'adresse ip et le port au socket
	if (bind(_serverSocket, _res->ai_addr, _res->ai_addrlen) == -1)
		handle_error("bind", 1);
	
	//ecouter via la socket pour détecter des demandes de connexion
	if (listen(_serverSocket, BACKLOG) == -1)
		handle_error("listen", 1);

	//mettre la socket en mode non bloquante
	setNonBlock(_serverSocket);
	
	//ajouter le server socket a la liste de socket
	pollfd serverFd = {_serverSocket, POLLIN, 0};
	_listSockets.push_back(serverFd);
}

void	HttpServer::acceptNewConnexion()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        else
            handle_error("accept", 1);
	}
	setNonBlock(clientSocket);
	pollfd clientFd = {clientSocket, POLLIN | POLLOUT, 0};
	_listSockets.push_back(clientFd);
}

std::string HttpServer::build_response() 
{
    return ("HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: 13\r\n"
           "\r\n"
           "Hello, World!");
}

void	HttpServer::run()
{
	while (true)
	{
		int status = poll(_listSockets.data(), _listSockets.size(), -1) == -1;
		if (status == -1)
			handle_error("poll", 1);
		acceptNewConnexion();
		if (_listSockets.size() > 1)
		{
			for (std::vector<struct pollfd>::iterator it = _listSockets.begin(); it != _listSockets.end(); ++it)
			{
				bool disconnect = false;
				if (it->revents & POLLIN & it->fd != _serverSocket)
				{
					char buffer[1024] = "\0";
					int ret = read(it->fd, buffer, 1024);
					if (ret == 0)
						disconnect = true;
					else if (ret < 0)
					{
						ret = true;
						std::cerr << "ERROR : read => " << strerror(errno) << "\n";
					}
					else
					{
						if (it->revents & POLLOUT)
						{
							std::cout << "Request received\n" << buffer << "\n";
							std::string response = build_response();
							ret = send(it->fd, response.c_str(), response.size(), 0);
							if (ret <= 0)
								disconnect = true;
						}
						
					}
					if (disconnect == true)
					{
						it = _listSockets.erase(it); 
						--it;
					}
				}
			}
		}	
	
	}
}

void	HttpServer::setNonBlock(int socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void	HttpServer::handle_error(const char *err, int i)
{
	if (!i)
		std::cerr << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
	if (_serverSocket != -1)
		close(_serverSocket);
	if (_res)
		freeaddrinfo(_res);
	exit(errno);
}