#include "HttpServer.hpp"

int main()
{
	std::string ip("127.0.0.1");
	std::string port("8080");
	HttpServer webserv(ip, port);

	webserv.init();
	webserv.run();
	return(0);
}
