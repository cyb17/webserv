/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:40:46 by yachen            #+#    #+#             */
/*   Updated: 2024/07/08 15:01:32 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Request.hpp"

class	Response
{
	private:
		
		std::string getGMTDate();
		std::string	build( int code, std::string& serverName, std::string body );

		
	public:

		Response();
		~Response();

		std::string	buildResponse( Request& request );

};

#endif


// response = "HTTP/1.1 200 OK\r\n" + date + server + "\r\nHello, World!";
// switch (infoLocation.allowMethods[i][0])
// {
// 	case 'G':
// 		response = responseGet(config, infos, infoLocation);
// 		break;
// 	case 'P':
// 		response = responsePost(config, infos, infoLocation);
// 		break;
// 	case 'D':
// 		response = responseDelete(config, infos, infoLocation);
// 		break;
// }

// std::string directoryRoot = config.root + config.location[i].root;
// DIR *dir = opendir(directoryRoot.c_str());
// if (dir == NULL)
	// return (response = "HTTP/1.1 404 Not Found\r\n" + date + server + "\r\nError: Not Found");
// closedir(dir);

// en commun:
// 	trouve le root qui correspond au chemin du dossier
// 		si oui, verifie si la methode est autorisee
// 		si non, retourne 404 Not Found
// GET:
// si le fichier demande est un dossier:
// 	si index.html oui, retourne le contenu de ce fichier
// 	si index.html non, ET que autoindex off, retourne 404 Not Found
// 	si index.html non, ET que autoindex on, retourne un listing de tous les sous repertoires et fichier
// 											qui sont des liens de redirections fonctionnels
// si le fichier demande est un fichier normal: 
// 	si existe, retourne les contenu de ce fichier
// 	si non, retourne 404 Not Found
// si le fichier demande est un executable cgi
// 	lance le child cgi proces qui genere un resultat
// 	retourne le resultat genere.
// POST:
// si le path se temine avec un fichier normal, retourne 405 Method Not Allowed
// si le path se temine avec un fichier cgi, execute le cgi (besoin ou non du content de POST selon le script choisi)
// 	si execution cgi succes retourne 200 OK,
// 	si non retourne 500 Internal Server Error
// si le path se termine avec un dossier, stock le content dans ce dossier.
// DELETE:
// suprime le dossier ou le fichier que pointe uri.