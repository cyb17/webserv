/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:27:44 by yachen            #+#    #+#             */
/*   Updated: 2024/06/28 17:39:43 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_EXTRACTOR_HPP
#define CONFIG_EXTRACTOR_HPP

#include <ConfigParser.hpp>

// typedef struct ErrorPage
// {
// 	int code;
// 	std::string	uri;
// }	errorPage;

// typedef struct Location
// {
// 	std::vector<std::string> allowMethods;
// 	std::string	autoindex;
// 	std::string root;
// 	std::string	index;
// 	std::string	redirection;
// }	Location;

// typedef struct Server
// {
// 	std::string	serverName;
// 	std::string	listen;
// 	std::string	host;
// 	int	clentMaxBodySize;
// 	std::string	root;
// 	std::string	index;
// 	std::vector<ErrorPage>	ErrorPage;
// 	std::vector<Location>	Location;
// }	Server;


class	ConfigExtractor
{
	private:

		ConfigExtractor( const ConfigExtractor& other );
		ConfigExtractor&	operator=( const ConfigExtractor& other );

	public:

		ConfigExtractor();
		~ConfigExtractor();

};

#endif