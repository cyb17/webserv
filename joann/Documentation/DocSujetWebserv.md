##  <p align =center><span style="color:lightyellow">PROJET WEBSERV - ANALYSE DU SUJET

|<div style="background-color:white"><span style="color:blue"> NOTIONS A RECHERCHER|   O/X   |		
|---                           |:-:      |
|Serveur WEB| <span style="color:green">O|
|RFC|X|
|Fichiers extension tpp et ipp|X|
|Fonctions externes autorisées| <span style="color:green">O|
|Poll(), select(), kqueue() ou epoll()|<span style="color:green">O|
|Serveur non bloquant| <span style="color:green">O|
|Macro FD_SET, FD_CLR, FD_ISSET et FD_ZERO|X|
|codes d’état de réponse HTTP|X| 
|CGI|X|
|Site web statique|<span style="color:green">O|
|Téléverser des fichiers|X|
|Méthodes GET, POST, et DELETE|X|
|Partie "serveur" du fichier de configuration NGINX|X|
|Taille du body des clients|X|
|Méthodes HTTP|X|
|Redirection HTTP|X|
|Listing des répertoires|X|

|<div style="background-color:white"><span style="color:green">**A FAIRE**   |   O/X   |		
|---                           |:-:      |
|<span style="color:lightgreen">PRE-REQUIS
|Ecrire en C++98|X|
|Binaire : “ /webserv [configuration file] ”|X|
|Lire la RFC|X|
|Choisir navigateur web|X|
|Avoir des pages d’erreur par défaut si aucune n’est fournie|X|
|Servir un site web entièrement statique|X|
|Téléverser des fichiers|X|
|<span style="color:lightgreen">FICHIER DE CONFIGURATION
|Choisir le port et l’host de chaque "serveur"|X|
|Setup server_names ou pas|X|
|Setup des pages d’erreur par défaut|X|
|Limiter la taille du body des clients|X|
|Définir une liste de méthodes HTTP acceptées pour la route|X|
|Définir une redirection HTTP|X|
|Définir un répertoire ou un fichier à partir duquel le fichier doit être recherché|X|
|Activer ou désactiver le listing des répertoires|X|
|Set un fichier par défaut comme réponse si la requête est un répertoire|X|
|Exécuter CGI en fonction de certaines extensions de fichier (par exemple .php)|X|
|Faites-le fonctionner avec les méthodes POST et GET|X|
|Rendre la route capable d’accepter les fichiers téléversés et configurer où cela doit être enregistré|X|

INFOS A RETENIR

Pre-requis
* poll() = select(), kqueue() ou epoll()
* ne pas utiliser d’autres serveurs web
* Serveur = non bloquant et utilisation d’un seul poll() (ou équivalent) pour toutes les opérations entrées/sorties entre le client et le serveur (listen inclus)
* poll() (ou équivalent) doit vérifier la lecture et l’écriture en même temps.
* Interdit de verifier la valeur errno après une opération de lecture ou d’écriture
* utilisation de read/recv ou write/send avec n’importe quel FD sans utiliser poll() (ou équivalent) => note = 0
* NGINX utilisé pour comparer les en-têtes et les comportements de réponse

Fichier de configuration
* s’inspirer de la partie "serveur" du fichier de configuration NGINX
* 1er serveur pour un host:port = serveur par défaut pour cet host:port (ce qui signifie qu’il répondra à toutes les requêtes qui n’appartiennent pas à un autre serveur).
* utiliser directement le chemin complet comme PATH_INFO a la place d’appeler le CGI
* 
* pour les requêtes fragmentées, votre serveur doit la dé-fragmenter et le CGI attendra EOF comme fin du body
* Même choses pour la sortie du CGI. Si aucun content_length n’est renvoyé par le CGI, EOF signifiera la fin des données renvoyées.
* Bien appeler le CGI avec le fichier demandé comme 1er argument
* Bien executer le cgi dans le bon repertoire pour l’acces au fichier de chemin relatif
* fonctionner avec un seul CGI (php-CGI, Python, etc.)