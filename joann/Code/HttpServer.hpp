/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/04 10:37:25 by jp-de-to         ###   ########.fr       */
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
# include "ConfigExtractor.hpp"

# define BACKLOG 1024

enum status
{
	CONNECT,
	DISCONNECT,
};

typedef struct infoServer 
{
    Server                      serverConfig;
    Request                     request;
} infoServer;

class HttpServer
{
    private :
        std::vector<Server>         _serverConfigLst;
        std::map<int, infoServer>   _infoServerLst;
        std::vector<struct pollfd>  _listSockets;

    public :
        
        HttpServer(std::vector<Server>& extract);
        ~HttpServer();

        // Setup all servers
        void setupAllServers();

        // Run all servers
        void runAllServers();
        void acceptNewConnexion(int serverSocket);
		status onRequestReceived(std::vector<struct pollfd>::iterator it);
		void displayMsgError(const char *err, int i);
		std::string build_response();
};

#endif