/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joannpdetorres <joannpdetorres@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/24 22:54:04 by joannpdetor      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSOCKET_HPP
# define TCPSOCKET_HPP

# include <sys/socket.h>
# include <iostream>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <netinet/in.h>
# include <netdb.h>
# include <vector>
# include <poll.h>

# define BACKLOG 1024

class TcpSocket
{
    private :
        std::string                 _adressIp;
        std::string                 _port;
        int                         _serverSocket;
        std::vector<struct pollfd>  _listSockets;
        addrinfo                    *_res;       

    public :
        TcpSocket(std::string &IpAdress, std::string &port);
        ~TcpSocket();

        void init();
        void run();
        void acceptNewConnexion();
        void handle_error(const char *err, int i);
};

#endif