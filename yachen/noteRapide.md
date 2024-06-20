<style>
r { color: Red }
o { color: Orange }
g { color: Green }
y { color: Yellow }
</style>

# Webserv
____

## Definitions : 

<o>**Hypertext**</o> : document which contents links to others documents, HTML is used to build a hypertext.  

<o>**HTTP**</o> : Hypertext Transfer Protocol, standard rules that define how Hypertext will be shared.

<o>**HTTP server**</o> : web server that receives HTTP requests and can send resources that it has stored.

<o>**World Wide We**</o> : information system that enables content sharing over the internet, making it  easily accessible to the public.

<o>**RFC**</o> : Requests For Comments,  a numbered series of documents describing the technical aspects and specifications of the Internet, or of different computer hardware.

<o>**NGINX**</o> : open source web server.

<o>**URL**</o> : Uniform Resource Locator, 

| https:// | www. | google.com | /path/to/file |
| -------- | -------- | -------- | -------- |
| protocol | subdomain | domain name | file path |

___

# Analyse du sujet :

## <r> Questions :</r> 

1.	Pourquoi les URLs commencent par HTTP ?
<br><y> Pour indiquer que le protocol utitilise pour acceder a une ressource de page web est HTTP. </y>

2.	Comment fonctionne un serveur HTTP ?
<br><y> En principe : il recoit une requete du client et lui repond soit la ressource demandee soit un code d'erreur.</y>
	<div>
		<y>Son deroulement : 
			<br>1.	configuration du serveur
			<br>2.	reception URL
			<br>3.	resolution DNS
			<br>4.	etablissement de connexion (si la ressource demandee se localise sur internet donc dans un autre serveur)
			<br>5.	recuperationde la ressource
			<br>6.	traite et envoie la ressource au client </y>
	<div><br>

3.	C'est quoi un fichier .ipp ?
<br><y> fichier utilise pour stocker les definitions de fonctions inline ou template </y>

**fonction inline* : des fonctions dont le code est inséré directement à l'endroit où elles sont appelées, afin de réduire l'overhead d'appel de fonction.

4.	C'est quoi un fichier de configuration pour un serveur web ?
<br><y>un fichier qui definit les parametres et le comportement du serveur</y>

5.	C'est quoi un socket et un nom du domaine ?
<br><y>un socket est une combinaison d'adresse IP et le numero de port</y>

6.	C'est quoi telnet ?

7.	
 

## <g>Les fonctions principales d'un serveur web : <g>

-	Stocker
-	Traiter
-	Livrer des pages Web aux clients


## <g>Fonctions externes autorisees : <g>

<o>socketpair</o> : cree une pair de socket connectee.

<o>htonl, htons</o> :  convertir un unsigned long ou un unsigned short de l'ordre des octets de l'hote a l'ordre des octets du reseau standard(big endian).

**big endian* : l'ordre de d'interpretation des octets, octets le plus significatif a gauche(memoire basse) et moins significatif a droite(meoire haute).

<o>ntohl, ntohs</o> : pareil que htonl et htons mais du reseau standard a l'hote.

<br>

