/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/14 11:24:16 by jp-de-to         ###   ########.fr       */
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
# include <cstdlib> 
# include <cctype>
# include <ctime> 
# include <iomanip>
# include <sys/types.h>
# include <dirent.h>
# include "ConfigExtractor.hpp"

enum CODE
{
	C200, //OK
	C301, //Moved Permanently
	C302, //Found
	C400, //bad request
	C403, //Forbidden
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
	std::string locationRoot;
	std::string locationFile;
	std::string version;
	std::string host;
	std::string contentType;
	std::string contentLength;
	int 		bodyLengthRequest;
	int			bodyLen;
	std::vector<std::string> body;
	//formulaire
	std::string queryString;
	//fichier televerse
	std::string	fileName;
	std::string fileBody;
} ResponsesInfos;

class	Request
{
	private:
		Server						_defaultConfigServer;
		Server						_configServer;
		int							_code;
		Step						_step;
		time_t						_startTime;
		std::vector<std::string>	_headersTmp;
		ResponseInfos 				_infos;
			
		bool		isGoodRequestLine( std::string& requestLine );
		bool		isGoodHeaders( std::vector<std::string>& headers );
		void		addInfos();

	public:

		Request();
		Request( Server& configServer, Server& defaultServer );
		~Request();

		void		printRequestInfos();
		//void		printServer(Server& info);
		Step		parseRequest( std::string& requestLine);

		//GETTERS
		int				getCode();
		time_t			getStartTime();
		ResponseInfos	getResponseInfos();
		Server 			getDefaultConfig();
		Server			getServerConfig();
};

#endif