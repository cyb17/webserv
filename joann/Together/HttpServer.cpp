

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:21:37 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/08 13:55:57 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

HttpServer::HttpServer(std::vector<Server>& extract, char** env) : _serverConfigLst(extract), _env(env) {}

HttpServer::~HttpServer() {}

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
			exitError("getaddrinfo", res, 0);

		int serverSocket = socket(res->ai_family,res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
		if (serverSocket == -1)
			exitError("socket", res, 1);
		
		if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1)
			exitError("bind", res, 1);
		
		if (listen(serverSocket, BACKLOG) == -1)
			exitError("listen", res, 1);

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
			diplayMsgError("accept");
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
	len = recv(it->fd, buffer, 1024, 0);
	if (len <= 0)
		return (DISCONNECT);
	buffer[len] = '\0';
	std::string tmp(buffer);
	std::cout << "\nREQUETE:\n" << tmp << "\n";
	if (_requestLst.empty() || _requestLst.find(it->fd) == _requestLst.end())
	{	
		Request request(_infoClientLst[it->fd], _serverConfigLst[0]);
		_requestLst.insert(std::make_pair(it->fd, request));
	}
	if ( _requestLst[it->fd].parseRequest(tmp) != complete)
	{
		time_t now;
		double secs;
		time(&now);
		secs = difftime(now, _requestLst[it->fd].getStartTime());
		if (secs > 120)
		{
			std::cerr << "Error: the customer request takes too long time to finalize\n";
			_requestLst.erase(it->fd);
			return (DISCONNECT);
		}
		return (CONNECT);
	}
	if (it->revents & POLLOUT)
	{
		Response	response( _env );
		std::string str = response.buildResponse( _requestLst[it->fd], *this );
		int	bytesent = send( it->fd, str.c_str(), str.size(), 0 );
		if (bytesent == -1)
			diplayMsgError("send");
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
			diplayMsgError("poll");
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
				else if (it->revents & POLLIN)	// ya un contenu a lire dans le socket client.
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

// lit le resultat du script cgi, le stock dans le body et met a jour le code HTTP.
int	HttpServer::readCgiResult( int fd, std::string& body )
{
	const size_t bufferSize = 10;
    char	buffer[bufferSize];
    ssize_t bytesRead;
    body.clear();
	bytesRead = read(fd, buffer, bufferSize);
    while (bytesRead > 0)
	{
        body.append(buffer, bytesRead);
		bytesRead = read(fd, buffer, bufferSize);
	}
    if (bytesRead == -1)
        return 500;
    return 200;
}

// cree un processus enfant pour executer le script externe, mettre a jour body le code HTTP.
int	HttpServer::executeCgi( std::string path, std::string& body )
{
	int	code = 200;
	int	pipefd[2];
	if (pipe( pipefd ) == -1)
		return 500;
	pid_t	pid = fork();
	if (pid == -1)
		return 500;
	else if (pid == 0)
	{
		close( STDIN_FILENO );
		close( pipefd[0] );
		dup2( pipefd[1], STDOUT_FILENO );
		close( pipefd[1] );
		const char*	argv[] = { path.c_str(), NULL };
		if (execve( path.c_str(), (char* const* )argv, _env ) == -1)
		{
			close( STDOUT_FILENO );
			exit(1);
		}
	}
	close( pipefd[1] );
	int status;
	waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return 403;
	code = readCgiResult( pipefd[0], body );
	close( pipefd[0] );
	return code;
}

void	HttpServer::exitError(std::string err, addrinfo *res, int i)
{
	if (res)
		freeaddrinfo(res);
	if (!i)
	{
		std::string tmp(gai_strerror(errno));
		throw std::invalid_argument(err + " => " + tmp);
	}
	else
	{
		std::string tmp(strerror(errno));
		throw std::invalid_argument(err + " => " + tmp);
	}
}

void	HttpServer::diplayMsgError(const char *err) { std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n"; }