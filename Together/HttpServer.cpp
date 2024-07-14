

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

HttpServer::HttpServer(std::vector<Server>& extract) : _serverConfigLst(extract) {}

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
		{
			close( serverSocket );
			exitError("bind", res, 1);
		}
		if (listen(serverSocket, BACKLOG) == -1)
		{
			close( serverSocket );
			exitError("listen", res, 1);
		}
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

void	HttpServer::createEnvCGI(ResponseInfos infos)
{
	const int envSize = 8;
	_env = new char*[envSize];
	std::string var[envSize - 1] = { "REQUEST_METHOD=" + infos.method,
        							"PATH_INFO=" + infos.locationRoot + infos.locationFile,
       								"CONTENT_TYPE=" + infos.contentType,
        							"CONTENT_LENGTH=" + infos.contentLength,
        							"QUERY_STRING=" + infos.queryString,
									"FILENAME=" + infos.fileName,
									"FILEBODY=" + infos.fileBody };
	for (int i = 0; i < envSize - 1; ++i) 
	{
        _env[i] = new char[var[i].size() + 1]; 
        std::strcpy(_env[i], var[i].c_str());
	}
	_env[envSize - 1] = NULL;
}

void HttpServer::freeEnv() 
{
	if (_env)
	{
    	for (int i = 0; _env[i] != NULL; ++i)
		{
    	    delete[] _env[i];
		}
		delete[] _env;
	}
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
	std::cout << "request: " << tmp << '\n';	
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
			return (_requestLst.erase(it->fd), DISCONNECT);
		return (CONNECT);
	}
	if (it->revents & POLLOUT)
	{
		createEnvCGI(_requestLst[it->fd].getResponseInfos());
		std::cout << "ENV:\n"; 
		for (int i = 0; i < 8; i++)
		{
			std::cout << _env[i] << "\n";	
		}
		Response	response( _env );
		std::string str = response.buildResponse( _requestLst[it->fd], *this );
		int	bytesent = send( it->fd, str.c_str(), str.size(), 0 );
		if (bytesent == -1)
			diplayMsgError("send");
	}
	freeEnv();
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
	const size_t bufferSize = 1024;
    char	buffer[bufferSize];
    ssize_t bytesRead;
    body.clear();
	bytesRead = read(fd, buffer, bufferSize);
    while (bytesRead > 0)
	{
        body.append(buffer, bytesRead);
		bytesRead = read(fd, buffer, bufferSize);
	}
	std::cout << "body: " << body << '\n';
    if (bytesRead == -1)
        return 500;
    return 200;
}

void	HttpServer::closeAllsSockets()
{
	while (!_listSockets.empty())
	{
		close( _listSockets.front().fd );
		_listSockets.erase( _listSockets.begin() );
	}
}

// cree un processus enfant pour executer le script externe, mettre a jour body le code HTTP.
int	HttpServer::executeCgi( std::string path, std::string& body )
{
	int	code = 200;
	int	pipefd[2], status[2];
	if (pipe( pipefd ) == -1)
		return 500;
	if (pipe( status ) == -1)
	{
		close( pipefd[1] );
		close( pipefd[0] );
		return 500;
	}
	pid_t	pid = fork();
	if (pid == -1)
	{
		close( pipefd[1] );
		close( pipefd[0] );
		close( status[1] );
		close( status[0] );
		return 500;
	}
	else if (pid == 0)
	{
		close(status[0]);
		close( STDIN_FILENO );
		close( pipefd[0] );
		dup2( pipefd[1], STDOUT_FILENO );
		close( pipefd[1] );
		const char*	argv[] = { path.c_str(), NULL };
		if (execve( path.c_str(), (char* const* )argv, _env ) == -1)
		{
			closeAllsSockets();
			freeEnv();
			dup2( status[1], STDOUT_FILENO );
			close( status[1] );
			throw std::runtime_error( "execve failed" );
		}
	}
	close( pipefd[1] );
	close( status[1] );
	std::string	childExitStatus;
	readCgiResult( status[0], childExitStatus );
	std::cout << "child exit status: " << childExitStatus << '\n';
	if (childExitStatus.empty())
		code = readCgiResult( pipefd[0], body );
	else
		code = 403;
	close( status[0] );
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