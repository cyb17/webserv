/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:40:46 by yachen            #+#    #+#             */
/*   Updated: 2024/07/09 13:45:25 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Request.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

class	Response
{
	private:
		
		char**	_env;
		
		std::string getGMTDate();
		std::string	build( int code, std::string& serverName, std::string body );
		int	executeCgi( std::string path,std::string& body );
		std::string	myGet( Server& config, ResponseInfos& infos );

		
	public:

		Response( char** env );
		~Response();

		std::string	buildResponse( Request& request );

};

#endif

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