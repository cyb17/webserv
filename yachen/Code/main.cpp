#include "HttpServer.hpp"
#include "../code/ConfigParser.hpp"
#include "../code/ConfigExtractor.hpp"

int	main( int argc, char** argv )
{
	if (argc != 2)
	{
		std::cout << "Error: invalid argument number\n";
		return 1;
	}
	try
	{
		ConfigParser	file( argv[1] );
		file.fillTokenList();
		file.analyseTokenList();

		ConfigExtractor	extrac;
		extrac.fillServerList( file.getTokenList() );
		extrac.printServerList();

		HttpServer servers(extrac.getServerList());
		servers.setupAllServers();
		servers.runAllServers();
	}
	catch (std::invalid_argument& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}

/*int main()
{
	std::string ip("127.0.0.1");
	std::string port("8080");
	HttpServer webserv(ip, port);

	webserv.init();
	webserv.run();
	return(0);
}*/
