#include <string>
#include <iostream>
#include <sstream>

int main()
{
	std::string empty = "\r\n";
	std::istringstream	fd( empty );
	std::string line;
	std::getline( fd, line );

	if ( line.empty() )
		std::cout << "yes\n";
}