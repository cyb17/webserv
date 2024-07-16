#include <iostream>
#include <sstream>
#include <string>

int	main()
{
	std::string	str("hello\r\n");
	std::istringstream	ss( str );
	std::string	line;
	std::getline( ss, line );
	if (line[line.size() - 1] == '\r')
		std::cout << "backslashr\n";
	std::cout << "line size : " << line.size() << '\n';

}