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
-	Extraire les informations. 		
-	Stocker les informations dans des structures de donnees.

objet : Server : contient :
	
	private:
		host
		port
		name
		root
		error_pages
		client_max_body_size
		methodes_accepted_for_root
		redirection
		listing
		defFile
	public:
		getHost
		getPort
		getName
		getRoot


2-	Connexion client - serveur : 

-	sockets programmation.

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