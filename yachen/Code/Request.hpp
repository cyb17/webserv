/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/03 17:53:46 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <fstream>
# include <sstream>
# include <string>
# include <map>
# include <vector>
#include "ConfigExtractor.hpp"

enum CODE
{
	C200, //OK
	C400, //bad request
	C404, //not found
	C500, //Internal Server Error
};

class	Request
{
	private:
		
		std::string	_request;
		Server		_configServer;
		int			_code;
			
		// bool	isGoodRequestLine( std::string& requestLine );
		// bool	isGoodHeaders( std::vector<std::string>& headers );
		// bool	isGoodBody( std::string& body );
		// void	parseRequest();

	public:

		Request( char str[], size_t size, Server& configServer);
		~Request();

		std::string	buildResponse();
};

// class Request
// {
// 	private:
// 		int									_socket;
// 		int									_step;
// 		std::string							_line;
// 		int									_responseCode;
// 		std::map<std::string, std::string> 	_firstLine;
// 		std::map<std::string, std::string>	_headers;
// 		std::vector<std::string>			_body;
	
// 	public:
// 		Request();
// 		Request(int socket);
// 		~Request();

// 		//MEMBER FUNCTIONS
// 		int parsing();
// 		int init();

// 		//CHECK FUNCTIONS
// 		bool checkFirstLine();
// 		bool checkHeaders();
		
// 		//SETTERS
// 		int setFirstLine();
// 		int	setHeaders();

// 		//GETTERS
// 		int getResponseCode();

// 		//PRINT FUNCTIONS
// 		void printFirstLine() const;
// 		void printHeaders() const;
// 		void printMsgError(const char *err);
// };

#endif