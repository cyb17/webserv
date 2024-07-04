#include <string>
#include <iostream>
#include <sstream>
#include <vector>


int	isGoodRequestLine( std::string& requestLine )
{
	std::istringstream	iss( requestLine );
	std::vector<std::string>	lineInfo;
	std::string word;
	while ( iss >> word )
		lineInfo.push_back( word );
	if (lineInfo.size() != 3 || requestLine.find( "\r\n", 0 ) == std::string::npos 
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE")
		|| lineInfo[2] != "HTTP/1.1")
	{
		return false;
	}
	else
	{
		return true;
		// info.method = lineInfo[0];
		// info.path = lineInfo[1];
		// info.version = lineINfo[2];
	}
}

int main()
{
	std::string line( "GET / HTTP/1.1   \r\n" );
	std::cout << isGoodRequestLine( line ) << '\n';
}