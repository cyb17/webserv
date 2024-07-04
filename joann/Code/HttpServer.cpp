/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/04 10:45:38 by jp-de-to         ###   ########.fr       */
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
	std::cout << "Creating Servers...\n";
	for (unsigned long i = 0; i < _serverConfigLst.size(); ++i)
	{
		addrinfo hints;
		addrinfo *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		if (getaddrinfo(_serverConfigLst[i].host.c_str(), _serverConfigLst[i].listen.c_str() , &hints, &res) != 0)
			displayMsgError("getaddinfo", 0);
		int serverSocket;
   		serverSocket = socket(res->ai_family,res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
   		if (serverSocket == -1)
			displayMsgError("socket", 1);

		if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1)
			displayMsgError("bind", 1);

		if (listen(serverSocket, BACKLOG) == -1)
			displayMsgError("listen", 1);

		pollfd serverFd = {serverSocket, POLLIN, 0};
		_listSockets.push_back(serverFd);

		infoServer	tmp;
		tmp.serverConfig = _serverConfigLst[i];
		_infoServerLst.insert(std::make_pair(serverSocket, tmp));

		std::cout << "Listening on " << _infoServerLst[serverSocket].serverConfig.host << ":" << _infoServerLst[serverSocket].serverConfig.listen << "\n";
		freeaddrinfo(res);
	}
}


void	HttpServer::acceptNewConnexion(int serverSocket)
{
	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof(client_addr);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr,  &addr_size);
	if (clientSocket == -1)
		return;
	pollfd clientFd = {clientSocket, POLLIN | POLLOUT, 0};
	_listSockets.push_back(clientFd);
}

status	HttpServer::onRequestReceived(std::vector<struct pollfd>::iterator it)
{

	char buffer[1024];
	int	len;

	len = recv(it->fd, buffer, 1024, 0);
	if (len <= 0)
		return (DISCONNECT);
	if (it->revents & POLLOUT)
	{
		int ret;
		std::string response = build_response();
		ret = send(it->fd, response.c_str(), response.size(), 0);
	}
	return (DISCONNECT);
}

std::string HttpServer::build_response() 
{
    return ("HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: 13\r\n"
           "\r\n"
           "Hello, World!");
}

void	HttpServer::runAllServers()
{
	int nb_server = _listSockets.size();
	while (true)
	{
		int pollStatus = poll(_listSockets.data(), _listSockets.size(), -1);
		if (pollStatus == -1)
			displayMsgError("poll", 1);
		else
		{
			for (int i = 0; i < nb_server; ++i)
				acceptNewConnexion(_listSockets[i].fd);
			for (std::vector<struct pollfd>::iterator it = _listSockets.begin() + nb_server; it != _listSockets.end(); ++it)
			{
				status client = CONNECT;
				if (it->revents == 0)
					continue;
				if (it->revents & POLLERR || it->revents & POLLHUP || it->revents & POLLNVAL)
					client = DISCONNECT;
				else if (it->revents & POLLIN)
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

//Errors

void	HttpServer::displayMsgError(const char *err, int i)
{
	if (!i)
		std::cerr << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}

