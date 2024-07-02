/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/29 13:44:10 by jp-de-to         ###   ########.fr       */
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

# define BACKLOG 1024

enum status
{
	CONNECT,
	DISCONNECT,
};

class HttpServer
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
};

#endif