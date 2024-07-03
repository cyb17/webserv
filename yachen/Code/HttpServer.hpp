/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/03 13:34:50 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include <sys/socket.h>
# include <cstdlib>
# include <iostream>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <netinet/in.h>
# include <netdb.h>
# include <vector>
# include <poll.h>
# include <fcntl.h>
# include "Request.hpp"
# include "../code/ConfigExtractor.hpp"

# define BACKLOG 1024

enum status
{
	CONNECT,
	DISCONNECT,
};

typedef struct infoServer 
{
    Server                      serverConfig;
    // Request                     request;
} infoServer;

typedef	struct infoClient
{
	Server						serverConfig;
	Request						request;
}

class HttpServer
{
    private :
		status						_status;
        std::vector<Server>         _serverConfigLst;// config data des servers.
        std::vector<struct pollfd>  _listSockets;// sockets clients ET sockets serveurs.
        std::map<int, infoServer>   _infoServerLst;// indice: fd socket server, valeur: config data du server.
		std::map<int, infoClient>	_infoClientLst;// indice: fd socket client, valeur: config fata du server correspondant.
		std::map<int, std::string>	_pendingResponse;// stock les reponse non renvoyer en raison de socket non toute suite dispo.

    public :
        
        HttpServer(std::vector<Server>& extract);
        ~HttpServer();

        void setupAllServers();
        void runAllServers();

		void diplayMsgError(const char *err, int i);
		void serverError(const char *err, int i);
        void acceptNewConnexion(int serverSocket);

		// void setNonBlock(int socket);		
		// std::string build_response();
		// void freeAndClose();
		// status onRequestReceived(std::vector<struct pollfd>::iterator it);
};

/*class HttpServer
{
    private :
        std::string                 _adressIp;
		status						_status;
        std::string                 _port;
        int                         _serverSocket;
        std::vector<struct pollfd>  _listSockets;
		std::map<int, Request>		_request;
        addrinfo                    *_res;

    public :
        
        HttpServer(std::string &IpAdress, std::string &port);
        ~HttpServer();

        void init();
        void run();
        void acceptNewConnexion();
		void setNonBlock(int socket);
		
		std::string build_response();
		void serverError(const char *err, int i);
		void diplayMsgError(const char *err, int i);
		void freeAndClose();
		status onRequestReceived(std::vector<struct pollfd>::iterator it);
};*/

#endif
