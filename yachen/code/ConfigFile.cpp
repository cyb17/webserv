/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:39:38 by yachen            #+#    #+#             */
/*   Updated: 2024/06/24 19:58:02 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigFile::ConfigFile( char* path ) : _path( path ) {}

ConfigFile::ConfigFile( const ConfigFile& other ) : _path( other._path ) {}

ConfigFile::~ConfigFile() {}

ConfigFile&	ConfigFile::operator=( const ConfigFile& other)
{
	_path = other._path;
	return *this;
}

void	ConfigFile::parse()
{
	std::fstream	fd( _path, std::fstream::in );
	if (!fd.is_open())
		throw std::invalid_argument( "open configuration file failed" );
	
	std::string	line;
	while (std::getline( fd, line ))
	{
		std::istringstream sstr( line );
		std::string	context;
		if (!sstr >> )
	}
	

}