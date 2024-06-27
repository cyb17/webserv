/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/27 23:27:24 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <errno.h>
# include <string.h>
# include <sys/socket.h>
# include <sstream>
# include <fstream>
# include <string>
# include <map>
# include <vector>

enum CODE
{
	C200, //OK
	C400, //bad request
	C404, //not found
	C500, //Internal Server Error
};

std::string firstline[3] = {"method", "url", "http"};

class Request
{
	private:
		int								_socket;
		int								_start;
		std::string						_line;
		int								_responseCode;
		std::map<std::string, std::string> 	_firstLine;
		std::map<std::string, std::string>	_headers;
		std::string						_body;
		Request();
	
	public:
		Request(int socket);
		~Request();

		//SETTERS
		int init();
		int Request::setStructRequest(char *buffer);
		void diplayMsgError(const char *err);
};

#endif