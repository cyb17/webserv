/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/06 15:00:01 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <sys/socket.h>
# include <ctime> 
# include <fstream>
# include <sstream>
# include <string>
# include <map>
# include <vector>
# include <cstdlib> 
# include <cctype>
# include "ConfigExtractor.hpp"

enum CODE
{
	C200, //OK
	C400, //bad request
	C404, //not found
	C405, //Method Not Allowed
	C408, //Request Timeout
	C500, //Internal Server Error
};

enum Step
{
	firstLine,
	headers,
	body,
	complete,
};
 
typedef struct ResponseInfos
{
	std::string	method;
	std::string path;
	std::string version;
	std::string host;
	int bodyLengthRequest;
	int	bodyLen;
	std::vector<std::string> body;
} ResponsesInfos;

class	Request
{
	private:
		// std::string	_request;
		Server						_configServer;
		int							_code;
		Step						_step;
		time_t						_startTime;
		ResponseInfos 				_infos;
		std::vector<std::string>	_headersTmp;
			
		bool	isGoodRequestLine(std::string& requestLine);
		bool	isGoodHeaders(std::vector<std::string>& headers);
		bool	isGoodBody( std::string& body );
		Step	parseRequest( std::string& requestLine);

	public:
		Request();
		Request( Server& configServer );
		~Request();

		void		printInfos();
		std::string	buildResponse(std::string& requestLine);

		//GETTERS
		Step			getStep();
		int				getCode();
		time_t			getStartTime();
		ResponseInfos	getResponseInfos();
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