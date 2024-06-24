/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPsocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joannpdetorres <joannpdetorres@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/06/24 15:39:54 by joannpdetor      ###   ########.fr       */
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

class TcpSocket
{
    private :
        std::string                 _IpAdress;
        int                         _port;
        int                         _serverSocket;
        std::vector<struct pollfd>  _tabSockets;            

    public :
        TcpSocket();
        TcpSocket(std::string IpAdress, int port);
        ~TcpSocket();

        void init();
        void run();
        void handle_error(char *err, int fd, int i);
};

#endif