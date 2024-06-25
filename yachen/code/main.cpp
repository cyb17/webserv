/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:27:37 by yachen            #+#    #+#             */
/*   Updated: 2024/06/25 17:02:23 by yachen           ###   ########.fr       */
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
	ConfigFile	file( argv[1] );
	file.makeTokenList();
	
	return 0;
}