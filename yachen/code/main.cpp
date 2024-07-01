/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:27:37 by yachen            #+#    #+#             */
/*   Updated: 2024/07/01 11:46:12 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ConfigParser.hpp"
#include "ConfigExtractor.hpp"

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
		file.printTokenList();

		ConfigExtractor	extrac;
		extrac.fillServerList( file.getTokenList() );
		extrac.printServerList();
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}