/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:27:37 by yachen            #+#    #+#             */
/*   Updated: 2024/06/24 20:00:22 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "ConfigFile.hpp"
using std::cout;

int	main( int argc, char** argv )
{
	if (argc != 2)
	{
		cout << "Error: invalid argument number\n";
		return 1;
	}
	ConfigFile	file( argv[1] );
	file.parse();
	
	return 0;
}