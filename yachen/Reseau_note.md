<style>
r { color: red }
o { color: Orange }
y { color: Yellow }
g { color: LightGreen }
lb{ color: LightBlue }
</style>
---
# <p style="text-align: center">RESEAU</p>

Le reseau permet a des machines interconnectees de pouvoir echanger des donnees.

Le reseau possede 3 types de <y>composants physiques</y> :
<br>-	<y>les peripheriques de terminaison</y> : ordinateurs, imprimantes, serveurs.
<br>-	<y>les equipements intermediaires</y> qui servent a interconnecter les peripheriques de terminaison entre eux sur le reseau : routeur et switch.
<br>-	<y>le support</y> qui transporte les donnees : cable , fibre optique.

---
### Les reseaux :
<o>PAN et WPAN</o> : Personal area network et Wireless Personal area network, reseau de quelque metre, interconnexion dans une piece (casque, souris...) 

<br><o>LAN et WLAN</o> : Wireless Local Area Network, reseau d'une zone restreinte (immeuble, batiment, maison...), interconnextion a l'aide des cables ethernets ou la WIFI.

<br><o>MAN</o> : Metropol Area Network, reseau metropolitain qui peut s'etendre a une 10zaine de km, couvre une ville entiere a l'aide des routeurs et des switchs.

<br><o>WAN</o> : Wide Area Network, permet d'interconnecter les LAN ou MAN grace a des infrastructures fibre optique, des cables sous-marins et des transmissions par satellite.

---

### IP : 

Une <y>adresse IP</y> est un numero <y>unique</y> attribue a un objet connecte a un reseau informatique. Il peut etre <y>public</y> ou <y>prive</y>(non routable sur internet) </g>

<o>IP</o> : <y>Internet Protocol</y> est utilise pour le routage et l'adressage des packets. 

---

### Modele de communication OSI :

7 couches : 

1-Physique (adresse physique): transfert de donnees en bits, cable ethernet(impulsions electriques), wifi(ondes radios), fibre optique(impulsions lumineuses).

2-Liaison de donnees (adresse MAC): carte reseau ou wifi (adresse MAC unique), regroupe et stock les donnees binaires dans un cadre(frame). Autre equipement : Switch : contient des interfaces reseaux(ports), tableaux d'adresses MAC, permet connexion des appareils sur un meme reseau local.

3-Reseau (adresse IP): routage des IP, trouve le bon chemin pour la couche 4

*difference entre 3 et 4 :
en adresses MAC : livraison de saut en saut (equipement en equipement, src et dest peut etre modifie pour atteindre dest final).
en adresse IP : livraison de bout en bout (src et dest non modifiees), en passant par des routeurs.

4-Transport (port): livraison de service a sercice, TCP(Transfer Control Protocol), UDP(User Datagram Protocol).

5-Session : initier une session avec destinataire afin d'echanger les donees.

6-Presentation : gere la syntaxe et le semantique des infos echangees pour qu'elles soient comprises des deux cotes.

7-Application : Elle fournit une interface pour les utilisateurs pour accéder aux services réseau et aux ressources partagées.


### Les Ports courants :

Port 80 (HTTP) : Utilisé pour les connexions web non sécurisées.
Port 443 (HTTPS) : Utilisé pour les connexions web sécurisées.
Port 25 (SMTP) : Utilisé pour l'envoi de courrier électronique.
Port 22 (SSH) : Utilisé pour l'accès distant sécurisé à des systèmes.
Port 20 et 21 (FTP) : Utilisé pour le transfert de fichiers via le protocole FTP.
Port 53 : domain Name System (DNS)