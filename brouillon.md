
# Idees
---
1-	Fichier de configuration :

-	Ecrire le fichier de configuration avec la syntaxe NGINX ou une syntaxe personnalisee .
-	Parser le fichier de configuration.
-	Extraire et stocker ces donnees.

2-	Connexion client - serveur : 

-	sockets programmation.

3-	Requetes HTTP : 

-	Parser la syntaxe et les en-tetes.
-	Extraire les infos.
-	Parser les infos extrait.
-	Traite la requete : statique ou dynamique(a l'aide de CGI).
-	Construit la reponse.
-	Retourner la reponses.


# RFC
---
-	7230 : syntaxes des messages HTTP, regles de traitement des messages, IANA considerations.

Format général :

	Requête : Request-Line suivie de header fields, une ligne vide (CRLF).
	Réponse : Status-Line suivie de header fields, une ligne vide (CRLF), et un corps de message (éventuellement).
	Champs d'En-Tête (Header Fields) : Chaque en-tête a un nom suivi de deux points et d'une valeur.
	Les lignes doivent se terminer par une séquence CRLF (Carriage Return suivi de Line Feed, \r\n).

Exemple requete client :

Request-Line : *Methode*, *URI*, *Version*

	GET /index.html HTTP/1.1

Header Fields :

	Host: www.example.com
	User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36
	Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
	Accept-Language: en-US,en;q=0.5
	Accept-Encoding: gzip, deflate, br
	Connection: keep-alive
	Upgrade-Insecure-Requests: 1

Exemple requete serveur :

Status-Line  : *Version*, *Code de statut*, *Raison*

	HTTP/1.1 200 OK

Header Fields :

	Date: Wed, 19 Jun 2024 10:00:00 GMT
	Server: Apache/2.4.41 (Ubuntu)
	Last-Modified: Tue, 18 Jun 2024 15:30:00 GMT
	Content-Length: 3495
	Content-Type: text/html; charset=UTF-8
	Connection: keep-alive

	<!DOCTYPE html>
	/* contenu... */
	</html>


Codes de Statut des Réponses :

	2xx (Succès) : Indiquent que la requête a été reçue, comprise, et acceptée. Exemple : 200 OK
	3xx (Redirections) : Indiquent que le client doit effectuer d'autres actions pour compléter la requête. Exemple : 301 Moved Permanently
	4xx (Erreurs Client) : Indiquent des erreurs dans la requête du client. Exemple : 404 Not Found
	5xx (Erreurs Serveur) : Indiquent que le serveur a échoué à traiter une requête valide. Exemple : 500 Internal Server Error
---

-	7231 : methodes de requetes, codes de statut des reponses, semantique des entites.
-	7232 : en-tetes de condition, validation et mise en cache, utilisation et comportement des caches.
-	7233 (peut etre pas besoin pour webserv) : spécifie les règles pour la récupération partielle de ressources HTTP à l'aide des en-têtes de plage.
-	7234 :  gestion des caches HTTP et spécifie les règles pour le stockage, la validation et l'invalidation des réponses en cache
-	7235 (peut etre pas besoin pour webserv): définit les règles d'authentification HTTP en spécifiant les schémas d'authentification et les en-têtes associés.


## CGI

---
Common Gateway Interface (littéralement « Interface de passerelle commune »), généralement abrégée CGI, est une interface utilisée par les serveurs HTTP.
Au lieu d'envoyer le contenu d'un fichier (fichier HTML, image), le serveur HTTP exécute un programme, puis retourne le contenu généré. 
