/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/25 09:02:53 by jp-de-to         ###   ########.fr       */
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

# define BACKLOG 1024

class HttpServer
{
    private :
        std::string                 _adressIp;
        std::string                 _port;
        int                         _serverSocket;
        std::vector<struct pollfd>  _listSockets;
        addrinfo                    *_res;

    public :
        HttpServer(std::string &IpAdress, std::string &port);
        ~HttpServer();

        void init();
        void run();
        void acceptNewConnexion();
		void handle_error(const char *err, int i);
		void setNonBlock(int socket);
		std::string build_response();
};

#endif