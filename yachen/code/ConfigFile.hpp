/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:10:52 by yachen            #+#    #+#             */
/*   Updated: 2024/06/27 14:36:02 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP
#include <vector>
#include <map>
#include <string>

enum tokenType
{
	DIRECTIVE = 1,
	PARAMETER = 2,
	BRACE_OP = 3,
	BRACE_CL = 4
};

typedef struct Token
{
	int	type;
	std::string value;
}	Token;

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
	std::string	redirection;
}	Location;

typedef struct Server
{
	std::string	serverName;
	std::string	listen;
	std::string	host;
	int	clentMaxBodySize;
	std::string	root;
	std::string	index;
	std::vector<ErrorPage>	vecErrorPage;
	std::vector<Location>	vecLocation;
}	Server;

class	ConfigFile
{
	private:

		char*	_path;
		std::vector<Token*>	_tokenList;
		std::vector<Server*>	_serverList;
		
		ConfigFile( const ConfigFile& other );
		ConfigFile&	operator=( const ConfigFile& other);
	

	public:

		ConfigFile( char* path );
		~ConfigFile();

		void	makeTokenList();
		void	checkBrace();
		void	analyseTokenList();
		// void	extractTokenListInfo();
		// const std::vector<Server>&	getServerList();
};


#endif

// for (size_t i = 0; i < directiveTab.size(); i++)
		// checkParameter( directiveTab + i );
	// std::cout << "_____________________directiveTab___________________\n";
	// for (size_t i = 0; i < directiveTab.size(); i++)
		// std::cout << directiveTab[i][0]->value << " | " << directiveTab[i][1]->value << '\n';
	// std::cout << "____________________________________________________\n";