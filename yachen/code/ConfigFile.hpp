/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:10:52 by yachen            #+#    #+#             */
/*   Updated: 2024/06/24 19:35:06 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP
#include <vector>
#include <map>
#include <string>

typedef struct ErrorPage
{
	int code;
	std::string	uri;
}	errorPage;

typedef struct Location
{
	std::vector<std::string>	allowMethods;
	std::string	autoindex;
	std::string	index;
}	Location;

typedef struct Server
{
	std::string	listen;
	bool	defaultServer;
	std::string	host;
	std::string	serverName;
	int		clentMaxBodySize;
	std::string	root;
	std::string	index;
	std::vector<ErrorPage>	vecErrorPage;
	std::vector<Location>	vecLocation;
}	Server;

class	ConfigFile
{
	private:

		char*	_path;
		std::vector<Server>	_vecServer;

		ConfigFile( const ConfigFile& other );
		ConfigFile&	operator=( const ConfigFile& other);

	public:

		ConfigFile( char* path );
		~ConfigFile();

		void	parse();
};


#endif
