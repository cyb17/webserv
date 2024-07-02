#include <string>

std::string	buildResponse( int code, std::string method, std::string path, std::string other )
{
	if (code == 400)
	 	return "Error: bad request"
}