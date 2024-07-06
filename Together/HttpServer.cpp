/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/06 17:59:35 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(std::vector<Server>& extract) : _serverConfigLst(extract)
{
	return ; 
}

HttpServer::~HttpServer() 
{
	return ; 
}

/*SETUP ALL SERVERS
Creer une boucle qui va creer une socket en mode ecoute pour chaque serveur indique
dans le fichier de configuration :
1. recuperer l'adresse IP et le port
2. creer une socket pour le serveur
3. lier l'adresse ip et le port au socket
4. ecouter via la socket pour détecter des demandes de connexion
5. ajouter le server socket a la liste de sockets
*/

void    HttpServer::setupAllServers()
{	
	for (unsigned long i = 0; i < _serverConfigLst.size(); ++i)
	{
		addrinfo hints;
		addrinfo *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if (getaddrinfo(_serverConfigLst[i].host.c_str(), _serverConfigLst[i].listen.c_str() , &hints, &res) != 0)
			serverError("getaddrinfo", 0);

		int serverSocket = socket(res->ai_family,res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
		if (serverSocket == -1)
			serverError("socket", 1);
		
		if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1)
			serverError("bind", 1);

		if (listen(serverSocket, BACKLOG) == -1)
			serverError("listen", 1);

		pollfd serverFd = {serverSocket, POLLIN, 0};
		_listSockets.push_back(serverFd);
		_infoServerLst[serverSocket] = _serverConfigLst[i];

		std::cout << "Listening on " << _infoServerLst[serverSocket].host << ":" << _infoServerLst[serverSocket].listen << "\n";
		freeaddrinfo(res);
	}
}

void	HttpServer::acceptNewConnexion(int serverSocket, Server& info)
{
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == -1)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			diplayMsgError("accept", 1);
		return;
	}
	pollfd clientFd = {clientSocket, POLLIN | POLLOUT, 0};
	_listSockets.push_back(clientFd);
	_infoClientLst[clientSocket] = info;	// associe la config serveur au socket client qui sera utile pour analyser la requete client.
}

status	HttpServer::onRequestReceived(std::vector<struct pollfd>::iterator it)
{
	int	len;
	char buffer[1024];
	len = recv(it->fd, buffer, 1023, 0);
	if (len <= 0)
		return (DISCONNECT);
	buffer[len] = '\0';
	std::string tmp(buffer);
	if (_requestLst.empty() || _requestLst.find(it->fd) == _requestLst.end())
	{	
		Request request(_infoClientLst[it->fd]);
		_requestLst.insert(std::make_pair(it->fd, request));
	}
	std::string response =  _requestLst[it->fd].buildResponse(tmp);
	
	if ( _requestLst[it->fd].getStep() != complete)
	{
		time_t now;
		double secs;
		time(&now);
		secs = difftime(now, _requestLst[it->fd].getStartTime());
		if (secs > 120)
		{
			std::cout << "Error: the customer request takes too long time to finalize\n";
			_requestLst.erase(it->fd);
			return (DISCONNECT);
		}
		return (CONNECT);
	}

	if (it->revents & POLLOUT)
	{
		int	bytesent = send( it->fd, response.c_str(), response.size(), 0 );
		if (bytesent == -1)
			diplayMsgError( "send" , 1 );
	}
	_requestLst.erase(it->fd);
	return (DISCONNECT);
}


void	HttpServer::runAllServers()
{
	int nbServer = _listSockets.size();
	while (true)
	{	// surveiller l'ensemble des fd par poll() pour des operation E/S.
		int pollStatus = poll(_listSockets.data(), _listSockets.size(), -1);
		if (pollStatus == -1)
			diplayMsgError("poll", 1);
		else
		{
			for (int i = 0; i < nbServer; ++i)
				acceptNewConnexion(_listSockets[i].fd, _infoServerLst[_listSockets[i].fd]);
			//parcourir tous les structures pollfd (server ET client) pour detecter des evenements.
			for (std::vector<struct pollfd>::iterator it = _listSockets.begin() + nbServer; it != _listSockets.end(); ++it)
			{
				status client = CONNECT;
				if (it->revents == 0)
					continue;
				if (it->revents & POLLERR || it->revents & POLLHUP || it->revents & POLLNVAL)
					client = DISCONNECT;
				else if (it->revents & POLLIN)	// ya des contenus a lire dans le socket client.
					client = onRequestReceived(it);
				if (client == DISCONNECT)
				{
					close(it->fd);
					it = _listSockets.erase(it); 
					--it;
				}
			}
		}
	}
}

void	HttpServer::serverError(const char *err, int i)
{
	diplayMsgError(err, i);
}

void	HttpServer::diplayMsgError(const char *err, int i)
{
	if (!i)
		std::cerr << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}

/*HttpServer::HttpServer(std::string &IpAdress, std::string &port) : _adressIp(IpAdress), _port(port) 
{
	_serverSocket = -1;
	return ; 
}

HttpServer::~HttpServer() 
{
	freeAndClose();
	return ; 
}*/

// void    HttpServer::init()
// {	
// 	//recuperer l'adresse IP et le port
// 	addrinfo hints;
// 	memset(&hints, 0, sizeof(hints));
// 	hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = AI_PASSIVE;
// 	if (getaddrinfo(_adressIp.c_str(), _port.c_str() , &hints, &_res) != 0)
// 		serverError("getaddrinfo", 0);
//     // creer une socket pour le serveur
//     _serverSocket = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
//     if (_serverSocket == 0)
// 		serverError("socket", 1);
// 	// lier l'adresse ip et le port au socket
// 	if (bind(_serverSocket, _res->ai_addr, _res->ai_addrlen) == -1)
// 		serverError("bind", 1);
// 	//ecouter via la socket pour détecter des demandes de connexion
// 	if (listen(_serverSocket, BACKLOG) == -1)
// 		serverError("listen", 1);
// 	//mettre la socket en mode non bloquante
// 	//setNonBlock(_serverSocket);
// 	//ajouter le server socket a la liste de sockets
// 	pollfd serverFd = {_serverSocket, POLLIN, 0};
// 	_listSockets.push_back(serverFd);
// }

// void	HttpServer::acceptNewConnexion()
// {
// 	int clientSocket = accept(_serverSocket, NULL, NULL);
// 	if (clientSocket == -1)
// 	{
// 		if (errno != EAGAIN && errno != EWOULDBLOCK)
// 			diplayMsgError("accept", 1);
// 		return;
// 	}
// 	setNonBlock(clientSocket);
// 	pollfd clientFd = {clientSocket, POLLIN | POLLOUT, 0};
// 	_listSockets.push_back(clientFd);
// }

// std::string HttpServer::build_response() 
// {
//     return ("HTTP/1.1 200 OK\r\n"
//            "Content-Type: text/plain\r\n"
//            "Content-Length: 13\r\n"
//            "\r\n"
//            "Hello, World!");
// }

// status	HttpServer::onRequestReceived(std::vector<struct pollfd>::iterator it)
// {
// 	if (_request.empty() || _request.find(it->fd) == _request.end())
// 		_request.insert(std::make_pair(it->fd, Request(it->fd)));
// 	Request& msg = _request[it->fd];
// 	if (!msg.parsing())
// 		return (CONNECT);
// 	std::cout << "CODE REPONSE = " << msg.getResponseCode() << "\n";
// 	msg.printFirstLine();
// 	if (it->revents & POLLOUT)
// 	{
// 		int ret;
// 		std::string response = build_response();
// 		ret = send(it->fd, response.c_str(), response.size(), 0);
// 		if (ret <= 0)
// 			_status = DISCONNECT;
// 	}
// 	_request.erase(it->fd);
// 	// sendResponse(it->fd);
// 	return (DISCONNECT);
// }

/*void	HttpServer::run()
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
}*/

/*void	HttpServer::setNonBlock(int socket)
{
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1)
		serverError("fcntl", 1);
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
		serverError("fcntl", 1);
}*/

//Errors



/*void	HttpServer::freeAndClose()
{
	if (_serverSocket != -1)
		close(_serverSocket);
	if (_res)
		freeaddrinfo(_res);
}*/


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