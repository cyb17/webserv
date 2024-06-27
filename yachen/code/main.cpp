/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:27:37 by yachen            #+#    #+#             */
/*   Updated: 2024/06/27 10:36:21 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ConfigFile.hpp"

int	main( int argc, char** argv )
{
	if (argc != 2)
	{
		std::cout << "Error: invalid argument number\n";
		return 1;
	}
	try
	{
		ConfigFile	file( argv[1] );
		file.makeTokenList();
		file.analyseTokenList();
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}

// if ((*it)->value == "location" && (*it)->type == DIRECTIVE)
				// {
				// 	while ((*(++it))->type != BRACE_OP)
				// 		server.push_back( *it );
				// 	std::vector<Token*>	location;
				// 	while ((*(++it))->type != BRACE_CL)
				// 		location.push_back( *it );
				// 	// checkLocationContent( location );
				// 	for (std::vector<Token*>::iterator it = location.begin(); it != location.end(); ++it)
				// 		std::cout << "type: "<< (*it)->type << " | value: " << (*it)->value << '\n';
				// 	std::cout << "--------------------------------------------------------------------\n";
				// }

				// else
		// {
		// 	std::cout << (*it)->value;
		// 	throw std::invalid_argument( "invalid directive present out of server block" );
		// }