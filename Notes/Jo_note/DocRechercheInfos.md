## Notions WEB

* **Navigateur WEB** = logiciel pour consulter et afficher le World Wide web (ex.: google chrome, mozilla firefox, etc ...)

* **Page WEB** = document qui peut etre affiché par un navigateur web

* **Site WEB** = ensemble de pages web regroupees entre elles de differentes facons <br/>
**=> Site web statique** : site web dont le contenu est figé et ne varie pas en fonction des caractéristiques de la demande. C’est-à-dire que les internautes qui demandent la page au même moment reçoivent le même contenu. <br/>
**=> Site web dynamique** : site internet qui affiche différents types de contenu chaque fois qu’un utilisateur le consulte vu que chaque mise à jour sur le site est automatique. <br/>
Cet affichage change en fonction d’un certain nombre de facteurs tels que les données démographiques de l’utilisateur, l’heure de la journée, le lieu, les paramètres linguistiques, etc.


## Qu'est-ce qu'un serveur web?

**Serveur WEB** = technologie qui stocke les fichiers (code et donnees) qui composent un site web et qui les envoie a l'appareil de l'utilisateur qui visite le site

* **DIFFERENCE ENTRE UN SERVEUR BLOQUANT ET NON BLOQUANT** <br/> 
	* **Serveur bloquant :** chaque requête est traitée de manière séquentielle et synchrone. Cela signifie que le serveur bloque (attend) jusqu'à ce que chaque opération soit terminée avant de passer à la suivante.<br/>

	**Fonctionnement :**
		1. Acceptation de la Connexion : Le serveur accepte une connexion entrante.
		2. Traitement de la Requête : Le serveur traite la requête en effectuant des opérations telles que la lecture de fichiers, l'accès à des bases de données, etc.
		3. Réponse à la Requête : Une fois le traitement terminé, le serveur envoie une réponse au client.
		4. Attente de la Prochaine Requête : Le serveur attend ensuite la prochaine requête.

* **Serveur non bloquant :** les opérations d'E/S sont effectuées de manière asynchrone, ce qui permet au serveur de continuer à traiter d'autres requêtes pendant qu'il attend la fin d'une opération.<br/>

	**Fonctionnement :**
	1. Acceptation de la Connexion : Le serveur accepte une connexion entrante. 
	2. Démarrage du Traitement de la Requête : Le serveur commence à traiter la requête, mais au lieu d'attendre que chaque opération se termine, il continue à accepter et traiter d'autres requêtes.
	3. Gestion des Opérations d'E/S Asynchrones: Le serveur utilise des mécanismes comme select, poll, epoll ou des callbacks pour être notifié lorsque des opérations d'E/S sont terminées.
	4. Envoi de la Réponse une Fois Prête : Lorsque l'opération d'E/S est terminée, le serveur finalise le traitement et envoie la réponse au client.


## Comment envoyer et recevoir des donnees a distance entre le navigateur et le serveur?

On va utiliser des **SOCKETS** pour etablir la connexion. <br/>

* **Qu’est-ce qu’un socket ?** <br/>

=> Descripteur de fichier qui permet de communiquer a distance. Il y a plusieurs types de sockets different, pour le sujet, on va s'interesser aux sockets internet. <br/>

Les 2 types principaux sont: <br/>
	-> **Stream sockets (SOCK_STREAM)**, qui se servent du protocole TCP pour communiquer. Ce protocole permet un transport des données fiable et connecté, au coût d’une performance réduite.  <br/>
-> **Datagram sockets (SOCK_DGRAM)**, qui utilisent le protocole UDP. Contrairement à TCP, UDP permet une transmission sans connexion, rapide mais sans garanties.
* **Etapes pour etablir la connexion entre le client et le serveur:** <br/>
	1. creer une socket "serveur_fd": <br/>
		socket() et strerror()
	2. initialiser la structure  stockadress:: <br/>
	getadressinfo() et gai_strerror()
	3. lier l'adresse ip et le port au socket: <br/>
	bind() et strerror()
	4. mettre la socket en attente de connexion: <br/>
	listen() et strerror()
	5. etablir la connexion en acceptant un appel qui va creer une socket pour le client: (client_fd) <br/>
	accept() et strerror()
	6. récupérer et surveiller l'état d'un ensemble de descripteurs de sockets: <br/>
	poll()

## Comment gerer les requetes et les reponses?

* **Proctocole de transfert hypertexte ou HTTP (HyperText Transfer Protocol)** <br/>

	* Une des 3 inventions qui ont permis lea creation du WEB

	* Protocole de communication permettant de récupérer et d’envoyer des ressources hypermédia <br/>

	* Base sur un modele client-serveur:<br/>
		**1.** Le navigateur va utiliser l'URL entré pour trouver l'adresse IP du serveur<br/>
		**2.** Le navigateur va envoyer une requete HTTP pour obtenir des informations<br/>
		**3.** Le serveur web va communiquer avec un serveur de base donnees pour trouver les donnees pertinentes<br/>
		**4.** Le serveur va renvoyer du contenu statique, tel que des pages HTML, des images, des vidéos ou des fichiers, sous forme de réponse HTTP au navigateur<br/>
		**5.** Le navigateur affiche ensuite le contenu de la page web <br/>

	* Serveur HTTP = port 80 et HTTPS = port 443 (par defaut)

* **Requete HTTP**

	* **Structure:**

		* 1er ligne -> preciser la requete :
			* methode de requete qui sert a indiquer le type de requete
			* cible de la requete: URL ou chemin absolu
			* version HTTP utilisee

		* lignes d'en-tetes HTTP (pas obligatoire)

		* ligne vierge

		* corps de requete (pas obligatoire)
	
		**exemple de requete:**<br/> 
		GET /XXX HTTP/1.1 <br/>
		Host: website.com <br/>
		User-Agent: Chrome/5.0 (Windows 10) <br/>
		Accept-Language: en-US <br/>
		Accept-Encoding: gzip, deflate <br/>
		Connection: keep-alive <br/>
	
	* **Methodes:**

		* Action que l'on souhaite realiser a la ressource indiquee, appelee aussi verbes HTTP

		* 9 methodes https existantes: GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH

		* Infos sur les 3 methodes indiquees dans le sujet (GET, POST, DELETE)

			* GET
				* Demande une representation de la ressource specifiee
				* Structure:
					* ligne de requete
					* en-tetes obligatoire: Host
					* pas de body
			
			* POST
				* Envoie des donnees au serveur
				* Structure :
					* ligne de requete
					* en-tetes obligatoires: Host, Content-Type, Content-Length
					* body
			
			* DELETE
				* Supprime la ressource indiquee
				* Structure :
					* ligne de requetes
					* en-tetes obligatoire: Host
