<style>
r { color: Red }
o { color: Orange }
g { color: Green }
y { color: Yellow }
</style>

# Idees :

1-	Fichier de configuration :

-	Ecrire un fichier de configuration.
-	Analyser le contenu fichier : syntaxe, directives et parametres.

	-	server
		-	listen
		-	host				
		-	serverName
		-	clientMaxBodySize
		-	root
		-	index
		-	defaultErrorPage
		-	locations
			-	allowMethods
			-	autoIndex;
			-	index
			-	redirection

syntaxe errors :	

	*	si des caracteres non whitespaces existe en dehors du server{}.
	*	si {} n'est pas correctement fermee.
	*	si une ligne de directives ne se termine pas par ; .

directives et ses parametres errors :

	*	si une directives non connue est utilisee.
	*	listen : si parametreNb < 0 || paratemeNb > 2 || portNumber != 0 - 65535 || 2eme parametre != "defaultServer" ou NULL.
	*	host : si ip non valid, ip=NULL et host sera parse par getaddrinfo, sinon ip sera parse par socketpair().
	*	serverName : caracteres autorise : alphanumerique et '_', ne doit pas commence par un chiffre.
	*	clientMaxBodySize :  doit etre entre 0 et 1000000.
	*	errorPages : 1 code 1 path
	*	location : directives autorises : allowMethods(GET, POST, DELETE), index(path), autoindex(on | off), return(301, URL)

-	Extraire les informations. 		
-	Stocker les informations dans des structures de donnees : utilise conteneur map


2-	Connexion client - serveur : 

-	sockets programmation : 

	*	socketpair() : domain = AF_INET(IPv4), type = SOCK_STREAM | SOCK_NONBLOCK, protocol = 0

3-	Requetes HTTP : 

-	Parser la syntaxe et les en-tetes.
-	Extraire les infos.
-	Parser les infos extrait.
-	Traite la requete : statique ou CGI(dynamique).
-	Construit la reponse.
-	Retourner la reponses.


*	les testes a faire avec NGINX : 

	*	ne met pas de port.
	*	mettre un site inconnu.