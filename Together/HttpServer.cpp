

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

HttpServer::HttpServer( std::vector<Server>& extract ) : _serverConfigLst( extract ) {}

HttpServer::~HttpServer() {}

// Creer une boucle qui va creer une socket en mode ecoute pour chaque serveur indique
// dans le fichier de configuration
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
			// recuperer l'adresse IP et le port
		if (getaddrinfo(_serverConfigLst[i].host.c_str(), _serverConfigLst[i].listen.c_str() , &hints, &res) != 0)
			exitError("getaddrinfo", res, 0);
			// creer une socket pour le serveur
		int serverSocket = socket(res->ai_family,res->ai_socktype | SOCK_NONBLOCK, res->ai_protocol);
		if (serverSocket == -1)
			exitError("socket", res, 1);
			// lier l'adresse ip et le port au socket
		if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1)
		{
			close( serverSocket );
			exitError("bind", res, 1);
		}	// ecouter via la socket pour détecter des demandes de connexion
		if (listen(serverSocket, BACKLOG) == -1)
		{
			close( serverSocket );
			exitError("listen", res, 1);
		}	// ajouter le server socket a la liste de sockets
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
		// associe au socket client la configuration du serveur qui l'ecoute, qui sera utile pour l'analyse requete client.
	_infoClientLst[clientSocket] = info;
}

status	HttpServer::onRequestReceived(std::vector<struct pollfd>::iterator client)
{
	int	len;
	char buffer[BUFFER_SIZE];

	len = recv(client->fd, buffer, BUFFER_SIZE - 1, 0);
	if (len <= 0)
		return (DISCONNECT);
	buffer[len] = '\0';
						
	std::string requestContent(buffer);
	std::cout << "\e[0;32mDEBUT REQUEST RECU: \n\e[0m" << requestContent << "\e[0;32m\nFIN REQUEST RECU\n\e[0m";
	if (_requestLst.empty() || _requestLst.find(client->fd) == _requestLst.end())
	{		// creer une nouvelle requete si la liste est vide ou requestContent n'a pas trouve de morceau precedent.
		Request request(_infoClientLst[client->fd], _serverConfigLst[0]);
		_requestLst.insert(std::make_pair(client->fd, request));
	}

	if (len < BUFFER_SIZE - 1)	// set _ndOfFullRequest == true
		_requestLst[client->fd].setEndOfFullRequest();

	if ( _requestLst[client->fd].parseRequest(requestContent) != complete)
	{		// configure un temps de limite pour rentrer les information d'une requete
		time_t now;
		double secs;
		time(&now);
		secs = difftime(now, _requestLst[client->fd].getStartTime());
		if (secs > 120)
			return (_requestLst.erase(client->fd), DISCONNECT);
		return (CONNECT);
	}

	if (client->revents & POLLOUT)
	{
		Response	response;
		std::string str = response.buildResponse( _requestLst[client->fd], *this );
		int	bytesent = send( client->fd, str.c_str(), str.size(), 0 );
		if (bytesent == -1)
			diplayMsgError("send");
	}
	_requestLst.erase(client->fd);
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

void	HttpServer::closeFds( int fd1, int fd2 )
{
	close( fd1 );
	close( fd2 );
}

char**	HttpServer::createEnvCGI( ResponseInfos& infos )
{	
	const int	size = 8;
	char**	env = new char*[size];
	std::string var[size - 1] = { 	"REQUEST_METHOD=" + infos.method,
        							"PATH_INFO=" + infos.locationRoot + infos.locationFile,
       								"CONTENT_TYPE=" + infos.contentType,
        							"CONTENT_LENGTH=" + infos.contentLength,
        							"QUERY_STRING=" + infos.queryString,
									"FILENAME=" + infos.fileName,
									"FILEBODY=" + infos.fileBody };
	for (int i = 0; i < size - 1; ++i) 
	{
        env[i] = new char[var[i].size() + 1]; 
        std::strcpy( env[i], var[i].c_str() );
	}
	env[size - 1] = NULL;
	return env;
}

void HttpServer::freeEnv( char**& env )
{
	if (env)
	{
    	for (int i = 0; env[i] != NULL; ++i)
    	    delete[] env[i];
		delete[] env;
	}
}

// cree un processus enfant pour executer le script externe, mettre a jour body le code HTTP.
int	HttpServer::executeCgi( std::string path, std::string& body, char**& env )
{
	int	code = 200;
	int	pipefd[2], status[2];
	if (pipe( pipefd ) == -1)
		return 500;
	if (pipe( status ) == -1)
		return closeFds( pipefd[0], pipefd[1] ), 500;
	pid_t	pid = fork();
	if (pid == -1)
		return closeFds( pipefd[0], pipefd[1] ), closeFds( status[0], status[1] ), 500;
	else if (pid == 0)
	{
		dup2( pipefd[1], STDOUT_FILENO );
		closeFds( status[0], STDIN_FILENO);
		closeFds( pipefd[0], pipefd[1] );
		const char*	argv[] = { path.c_str(), NULL };
		if (execve( path.c_str(), (char* const* )argv, env ) == -1)
		{
			closeAllsSockets();
			freeEnv( env );
			dup2( status[1], STDOUT_FILENO );
			close( status[1] );
			throw std::runtime_error( "execve failed" );
		}
	}
	closeFds( pipefd[1], status[1] );
	std::string	childExitStatus;
	readCgiResult( status[0], childExitStatus );
	if (childExitStatus.empty())
		code = readCgiResult( pipefd[0], body );
	else
		code = 403;
	closeFds( pipefd[0], status[0] );
	freeEnv( env );
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

void	HttpServer::diplayMsgError(const char *err)
{ 
	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
}