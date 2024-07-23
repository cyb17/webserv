#include "HttpServer.hpp"
#include "ConfigParser.hpp"
#include "ConfigExtractor.hpp"

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

		HttpServer servers(extrac.getServerList());
		servers.setupAllServers();
		std::cout << GREEN <<"\nALL SERVERS INITIALIZED WITH SUCCES...\n\n" << RESET;
		servers.runAllServers();
	}
	catch (std::invalid_argument& e)
	{
		std::cerr << RED << "Error: " << e.what() << std::endl << RESET;
	}
	catch (std::runtime_error& ex)
	{

		std::cout << RED << "Error: " << ex.what() << std::endl << RESET;
	}
	return 0;
}
