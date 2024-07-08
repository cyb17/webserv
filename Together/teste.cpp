#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

int	makeBody( std::string path, std::string& body )
{
	std::ifstream file(path.c_str());
	if (!file)
	{
		body = "Forbidden\r\n";
		return (403);
	}
	std::string line;
	while (std::getline(file, line))
		body += line;
	return (200);
}

int main()
{
	std::string path("/mnt/nfs/homes/jp-de-to/hi.html");
	std::string body;
	int code = makeBody(path, body);

	std::cout << "code = " << code << "\n";
	std::cout << body;
	return 0;
}