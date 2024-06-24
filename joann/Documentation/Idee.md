# Brainstorming

* **Connexion entre le serveur et le client**
    * creer une class TcpSocket

        * infos importants : 
            * adresse ip
            * port
            * server socket (nb max = 1)
            * clients socket (nb max = 1024)
        
        * fonctions a creer
            * initialiser: serveur socket 
            * executer le serveur en creant une boucle:
                * creer un tableau de socket 
                * mettre le serveur en mode non bloquant
                * ajouter une connexion client
                * requete client
                * reponse serveur
    
    * gerer les erreurs