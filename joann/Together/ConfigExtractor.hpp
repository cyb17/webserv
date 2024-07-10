/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:27:44 by yachen            #+#    #+#             */
/*   Updated: 2024/07/01 11:41:35 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_EXTRACTOR_HPP
#define CONFIG_EXTRACTOR_HPP

#include "ConfigParser.hpp"

typedef struct Location
{
	std::string	autoindex;
	std::string root;
	std::string	index;
	std::vector<std::string> 	allowMethods;
	std::pair<int, std::string>	redirection;
}	Location;

typedef struct Server
{
	std::string	serverName;
	std::string	listen;
	std::string	host;
	std::string	root;
	std::string	index;
	int							clientMaxBodySize;
	std::map<int, std::string>	errorPages;
	std::vector<Location>		location;
}	Server;

class	ConfigExtractor
{
	private:

		std::vector<Server>	_serverList;
		Server				_defaultServer;

		ConfigExtractor( const ConfigExtractor& other );
		ConfigExtractor&	operator=( const ConfigExtractor& other );

		Location					fillLocationData( std::vector<Token*>& tokens );
		std::map<int, std::string>	fillErrorPageData( std::vector<Token*>& tokens );
		Server						fillServerData( std::vector<Token*>& tokens );

	public:

		ConfigExtractor();
		~ConfigExtractor();

		void					fillServerList( std::vector<Token*>& tokens );
		std::vector<Server>&	getServerList();
		Server&					getDefaultServer();
		void					printServerList();
		

};

#endif