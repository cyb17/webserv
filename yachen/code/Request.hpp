/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/04 14:23:36 by yachen           ###   ########.fr       */
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

typename struct ReponseInfo
{
	std::string	methods;
	std::string path;
	std::string version;
	std::vector<string>	body;

} ReponseInfo;

class	Request
{
	private:
		
		std::string	_request;
		Server		_configServer;
		int			_code;
			
		bool	isGoodRequestLine( std::string& requestLine, ReponseInfo& info );
		bool	isGoodHeaders( std::vector<std::string>& headers );
		bool	isGoodBody( ReponseInfo );
		void	parseRequest( ReponseInfo );

	public:

		Request( char str[], size_t size, Server& configServer);
		~Request();

		std::string	buildResponse();
};

#endif