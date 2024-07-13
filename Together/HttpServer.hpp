/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joannpdetorres <joannpdetorres@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:33:43 by joannpdetor       #+#    #+#             */
/*   Updated: 2024/07/13 16:50:19 by joannpdetor      ###   ########.fr       */
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
# include "Response.hpp"
# include <stdexcept>

# define BACKLOG 1024

enum status
{
	CONNECT,
	DISCONNECT,
};

class HttpServer
{
    private :
	
        std::vector<Server>         _serverConfigLst;	// configurations des servers.
        std::vector<struct pollfd>  _listSockets;	// sockets clients ET serveurs.
        std::map<int, Server>  		_infoServerLst;	// indice: fd socket server, valeur: configuration du server correspondant.
		std::map<int, Server>		_infoClientLst;	// indice: fd socket client, valeur: configuration du server correspondant.
		std::map<int, Request>		_requestLst; // ind: fd socket client, val: requete du client
		char**						_env;

		void	acceptNewConnexion(int serverSocket, Server& info);
		status	onRequestReceived(std::vector<struct pollfd>::iterator it);
		void	createEnvCGI(ResponseInfos infos);
		void	freeEnv();
		int		readCgiResult( int fd, std::string& body );
		void 	diplayMsgError(const char *err);
		void 	exitError(std::string err, addrinfo *res, int i);
		void	closeAllsSockets();
		
		public :
			
			HttpServer(std::vector<Server>& extract, char** env);
			~HttpServer();
			
			void 	setupAllServers();
			void	runAllServers();
			int		executeCgi( std::string path, std::string& body );
			
};

#endif
