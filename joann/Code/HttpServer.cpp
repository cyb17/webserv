/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/29 13:49:42 by jp-de-to         ###   ########.fr       */
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
	freeAndClose();
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
		serverError("getaddrinfo", 0);
    // creer une socket pour le serveur
    _serverSocket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
    if (_serverSocket == 0)
		serverError("socket", 1);
	// lier l'adresse ip et le port au socket
	if (bind(_serverSocket, _res->ai_addr, _res->ai_addrlen) == -1)
		serverError("bind", 1);
	//ecouter via la socket pour détecter des demandes de connexion
	if (listen(_serverSocket, BACKLOG) == -1)
		serverError("listen", 1);
	//mettre la socket en mode non bloquante
	setNonBlock(_serverSocket);
	//ajouter le server socket a la liste de sockets
	pollfd serverFd = {_serverSocket, POLLIN, 0};
	if (_listSockets.empty())
		_listSockets.push_back(serverFd);
}

void	HttpServer::acceptNewConnexion()
{
	int clientSocket = accept(_serverSocket, NULL, NULL);
	if (clientSocket == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			diplayMsgError("accept", 1);
		return;
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

// void	HttpServer::run()
// {
// 	while (true)
// 	{
// 		int status = poll(_listSockets.data(), _listSockets.size(), -1) == -1;
// 		if (status == -1)
// 			diplayMsgError("poll", 1);
// 		acceptNewConnexion();
// 		if (_listSockets.size() > 1)
// 		{
// 			for (std::vector<struct pollfd>::iterator it = _listSockets.begin(); it != _listSockets.end(); ++it)
// 			{
// 				_status = CONNECT;
// 				if (it->revents & POLLIN & it->fd != _serverSocket)
// 				{
// 					char request[1024];
// 					ssize_t ret = read(it->fd, request, 1024);
// 					if (ret == 0)
// 						_status = DISCONNECT;
// 					else if (ret < 0)
// 					{
// 						_status = DISCONNECT;
// 						diplayMsgError("read", 1);
// 					}
// 					else
// 					{
// 						if (it->revents & POLLOUT)
// 						{
// 							std::cout << "Request received\n" << request << "\n";
// 							std::string response = build_response();
// 							ret = send(it->fd, response.c_str(), response.size(), 0);
// 							if (ret <= 0)
// 								_status = DISCONNECT;
// 						}
// 					}
// 					if (_status == DISCONNECT)
// 					{
// 						close(it->fd);
// 						it = _listSockets.erase(it); 
// 						--it;
// 					}
// 				}
// 			}
// 		}
// 	}
// }

status	HttpServer::onRequestReceived(std::vector<struct pollfd>::iterator it)
{
	if (_request.empty() || _request.find(it->fd) == _request.end())
		_request.insert(std::make_pair(it->fd, Request(it->fd)));
	Request& msg = _request[it->fd];
	if (!msg.parsing())
		return (CONNECT);
	std::cout << "CODE REPONSE = " << msg.getResponseCode() << "\n";
	msg.printFirstLine();
	if (it->revents & POLLOUT)
	{
		int ret;
		std::string response = build_response();
		ret = send(it->fd, response.c_str(), response.size(), 0);
		if (ret <= 0)
			_status = DISCONNECT;
	}
	_request.erase(it->fd);
	// sendResponse(it->fd);
	return (DISCONNECT);	
}

void	HttpServer::run()
{
	while (true)
	{
		int status = poll(_listSockets.data(), _listSockets.size(), -1) == -1;
		if (status == -1)
		{
			diplayMsgError("poll", 1);
			continue ;
		}
		acceptNewConnexion();
		if (_listSockets.size() > 1)
		{
			for (std::vector<struct pollfd>::iterator it = _listSockets.begin(); it != _listSockets.end(); ++it)
			{
				_status = CONNECT;
				if ((it->revents &POLLIN) && it->fd != _serverSocket)
				{
					_status = onRequestReceived(it);
					if (_status == DISCONNECT)
					{
						close(it->fd);
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
	if (flags == -1)
		serverError("fcntl", 1);
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
		serverError("fcntl", 1);
}

//Errors

void	HttpServer::serverError(const char *err, int i)
{
	diplayMsgError(err, i);
	freeAndClose();
	exit(errno);
}

void	HttpServer::diplayMsgError(const char *err, int i)
{
	if (!i)
		std::cerr << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}

void	HttpServer::freeAndClose()
{
	if (_serverSocket != -1)
		close(_serverSocket);
	if (_res)
		freeaddrinfo(_res);
}
