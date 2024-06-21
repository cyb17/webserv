#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080
#define HOST "www.exemple.com"
#define BACKLOG 1024

void handle_error(char *err, int fd, int i)
{
	if (!i)
		std::cout << "ERROR : " << err << " => " << gai_strerror(errno) << "\n";
	else
		std::cout << "ERROR : " << err << " => " << strerror(errno) << "\n";
	if (fd > -1)
		close(fd);
	exit(errno);
}

int main()
{
	int server_fd, client_fd;
	int status;
	sockaddr_in soc_addr;
	addrinfo hints;
	addrinfo *res;

	//recuperer l'adresse ip
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;
	status = getaddrinfo(HOST, 0, &hints, &res);
	if (status != 0)
		handle_error("getaddrinfo", -1, 0);

	//creation du socket serveur
	server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); 
	if (server_fd == -1)
		handle_error("socket", server_fd, 1);

	//Lier la socket à l’addresse IP
	status = bind(server_fd, res->ai_addr, res->ai_addrlen);
	if (status != 0)
		handle_error("bind", server_fd, 1);

	//Écouter via la socket pour détecter des demandes de connexion


	close(server_fd);
	return(0);
}
