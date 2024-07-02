/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:27:37 by yachen            #+#    #+#             */
/*   Updated: 2024/07/02 17:00:36 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ConfigParser.hpp"
#include "ConfigExtractor.hpp"
#include "../code_joann/HttpServer.hpp"

int	main( int argc, char** argv )
{
	if (argc != 2)
	{
		std::cout << "Error: invalid argument number\n";
		return 1;
	}
	try
	{
		ConfigParser	file( argv[1] );
		file.fillTokenList();
		file.analyseTokenList();
		// file.printTokenList();

		ConfigExtractor	extrac;
		extrac.fillServerList( file.getTokenList() );
		// extrac.printServerList();
		std::vector<Server>	serverList( extrac.getServerList() );
		std::vector<HttpServer>	webservList;
		for (size_t i = 0; i < serverList.size(); ++i)
		{
			HttpServer	webserv( serverList[i].host, serverList[i].listen );
			webserv.init();
			webserv.run();
		}
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}