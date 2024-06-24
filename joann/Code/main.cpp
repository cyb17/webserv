#include "TcpSocket.cpp"

int main()
{
	std::string ip = "127.0.0.1";
	std::string port = "8080";
	TcpSocket webserv(ip, port);

	webserv.init();
	//webserv.run();
	return(0);
}
