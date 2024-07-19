#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

int	main(int ac, char** av, char** env)
{
	std::cout << ac << '\n';
	execve( "./webSite/cgi-bin/scriptCgi.py", av, env );
}