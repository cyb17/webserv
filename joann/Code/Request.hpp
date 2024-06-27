/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/27 11:57:27 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <errno.h>
# include <string.h>
# include <sys/socket.h>
# include <string>
# include <map>
# include <vector>
# include "HttpServer.hpp";

enum CODE
{
	C200, //OK
	C400, //bad request
	C404, //not found
	C500, //Internal Server Error
};

class Request
{
	private:
		int								_socket;
		bool							_valid;
		std::map<char *, std::string> 	_firstLine;
		std::map<char *, std::string>	_headers;
		std::string						_body;
		Request();
	
	public:
		Request(int socket);
		~Request();

		//SETTERS
		Connexion setFirstline();
		Connexion init();
		Connexion checkConnexion();
		void diplayMsgError(const char *err);
};

#endif