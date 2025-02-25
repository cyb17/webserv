/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/23 16:47:38 by yachen           ###   ########.fr       */
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
	std::string host;
	std::string queryString;
	
	std::string contentType;
	std::string contentLength;
	int 		bodyLengthRequest;
	int			bodyLen;
	std::vector<std::string> body;
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
		bool						_endOfFullRequest;
		std::vector<std::string>	_headersTmp;
		ResponseInfos 				_infos;
			
		bool		isGoodRequestLine( std::string& requestLine );
		bool		isGoodHeaders( std::vector<std::string>& headers );
		void		addInfos();

	public:

		Request();
		Request( Server& configServer, Server& defaultServer );
		~Request();

		Step			parseRequest( std::string& requestLine);

		void			setEndOfFullRequest();

		int				getCode();
		time_t			getStartTime();
		bool			getEndOfFullRequest();
		ResponseInfos	getResponseInfos();
		Server 			getDefaultConfig();
		Server			getServerConfig();
};

#endif