# Fonctions externes autorisees

* **execve** <unistd.h>
	* Prototype :
		* int execve(const char *filename, char *const argv[], char *const envp[]) 
	* Description : exécute le programme correspondant au fichier filename.

* **dup, dup2** <unistd.h>
	* Prototype :
		* int dup(int oldfd)
		* int dup2(int oldfd, int newfd)
	* Description : cree une copie du descripteur de fichier "oldfd".

* **pipe** <unistd.h>
	* Prototype :
		* int pipe(int pipefd[2]);
	* Description : crée un tube, un canal unidirectionnel de données qui peut être utilisé pour la communication entre processus.

* **fork** <unistd.h>
	* Prototype :
		* pid_t fork(void);
	* Description : crée un nouveau processus en copiant le processus appelant.

* **strerror** <string.h>
	* Prototype :
		* char *strerror(int errnum)
	* Description : renvoie un pointeur vers une chaîne qui décrit le code d'erreur passé en argument errnum

* **gai_strerror** <netdb.h>
	* Prototype :
		* const char *gai_strerror(int ecode)
	* Description : traduit des codes d'erreur retournés par les fonctions de la famille getaddrinfo en une chaîne de caractères compréhensible, utilisable pour rendre compte du problème 

* **errno** <errno.h>
	* Description : Variable globale qui contient le dernier code d'erreur

* **socketpair** <sys/socket.h>
	* Prototype :
		*  int socketpair(int domain, int type, int protocol, int sv[2])
	* Description : Crée une paire de sockets connectés entre eux.

* **poll, select** <poll.h> <sys/select.h> 
	* Prototype :
		* int poll(struct pollfd *fds, nfds_t nfds, int délai)
		* int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
	* Description : Mécanismes pour gérer les E/S multiplexées, cad gérer plusieurs connexions réseaux simultanées dans un serveur web

* **socket** <sys/socket.h>
	* Prototype :
		* int socket(int domain, int type, int protocol)
	* Description : crée un point de communication, et renvoie un descripteur

* **accept** <sys/socket.h>
	* Prototype :
		* int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	* Description : Accepter une connexion sur une socket

* **listen** <sys/socket.h>
	* Prototype :
		* int listen(int sockfd, int backlog)
	* Description : Attendre des connexions sur une socket 

* **send** include <sys/socket.h>
	* Prototype :
		* ssize_t send(int s, const void *buf, size_t len, int flags)
	* Description : Envoyer un message sur une socket  

* **bind** <sys/socket.h>
	* Prototype :
		* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
	* Description : Fournir un nom à une socket 

* **connect** <sys/socket.h>
	* Prototype :
		* int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
	* Description : Débuter une connexion sur une socket 

* **getaddrinfo** <sys/socket.h>
	* Prototype :
		* int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
	* Description : convertir les noms de domaine ou les adresses de service en structures d'adresse socket

* **freeaddrinfo** <sys/socket.h>
	* Prototype :
		* void freeaddrinfo(struct addrinfo *res)
	* Description : libérer la mémoire allouée par la structure getaddrinfo

* **setsockopt** <sys/socket.h>
	* Prototype :
		* int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
	* Description : définir les options d'une socket

* **getsockname** <sys/socket.h>
	* Prototype :
		* int getsockname(int socket, struct sockaddr *address, socklen_t *address_len)
	* Description : obtenir l'adresse locale associée à une socket.

* **getprotobyname** <sys/socket.h>
	* Prototype :
		* struct protoent *getprotobyname(const char *name)
	* Description : obtenir des informations sur un protocole en utilisant son nom

* **fcntl** <unistd.h> <fcntl.h>
	* Prototype :
		* int fcntl(int fd, int cmd, ... /* arg */ )
	* Description : manipuler les descripteurs de fichiers dans les systèmes Unix/Linux

* **close** <unistd.h>
	* Prototype :
		* int close(int fd)
	* Description : ferme un descripteur de fichier, rendant ainsi la ressource de fichier associée disponible pour d'autres utilisations

* **read** <unistd.h>
	* Prototype :
		* ssize_t read(int fd, void *buf, size_t count)
	* Description : lit des données à partir d'un descripteur de fichier dans un tampon

* **write** <unistd.h>
	* Prototype :
		* ssize_t write(int fd, const void *buf, size_t count)
	* Description : écrit des données à partir d'un tampon vers un descripteur de fichier

* **waitpid** <sys/types.h>
	* Prototype :
		* pid_t waitpid(pid_t pid, int *status, int options)
	* Description : attend la fin d'un processus enfant spécifié


* **access** <unistd.h>
	* Prototype :
		* int access(const char *pathname, int mode)
	* Description : vérifie l'existence du fichier et les permissions d'accès

* **stat** <unistd.h>
	* Prototype :
		* int stat(const char *pathname, struct stat *statbuf)
	* Description : obtient des informations sur un fichier

* **open** <sys/types.h> <fcntl.h>
	* Prototype :
		* int open(const char *pathname, int flags, ... /* mode_t mode */)
	* Description : ouvre un fichier et retourne un descripteur de fichier

* **opendir** <sys/types.h>
	* Prototype :
		* DIR *opendir(const char *name)
	* Description :  ouvre un répertoire et retourne un pointeur vers une structure DIR

* **readdir** <dirent.h>
	* Prototype :
		* struct dirent *readdir(DIR *dirp)
	* Description : lit une entrée de répertoire à partir d'un répertoire ouvert

* **closedir** <dirent.h> <sys/types.h>
	* Prototype :
		* int closedir(DIR *dirp)
	* Description : ferme un répertoire ouvert