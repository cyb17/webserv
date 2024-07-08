/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/08 13:47:45 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}
Response::~Response() {}

std::string Request::getGMTDate()
{
	time_t		currentTime;
	struct tm*	timeInfos;
	const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time( &currentTime );
	timeInfos = gmtime( &currentTime );
	std::stringstream	ss;
	
    ss  << days[timeInfos->tm_wday] << ", "
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_mday << " "
    	<< months[timeInfos->tm_mon] << " "
    	<< (timeInfos->tm_year + 1900) << " "
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_hour << ":"
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_min << ":"
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_sec
    	<< " GMT";
	return ss.str();
}

std::string	Response::buildResponse( Request& request )
{
	std::string response;
	std::string date = "Date: " + getGMTDate() + "\r\n";
	std::string server = "Server: " + _configServer.serverName + "\r\n";
	if (_code == 400)
		response = "HTTP/1.1 400 Bad Request\r\n" + date + server + "\r\nError: Bad Request";
	else
	{	
		ResponseInfos infoResponse = getResponseInfos();
		Server		infoServer = _configServer;

		// check server name
		if (infoResponse.host != infoServer.serverName)
			infoServer = _defaultConfigServer;
		
		// check root
		unsigned long i = 0;
		while (i < infoServer.location.size())
		{
			if (infoResponse.locationRoot == infoServer.location[i].root)
				break;
			i++;
		}
		if (i == infoServer.location.size())
			return (response = "HTTP/1.1 404 Not Found\r\n" + date + server + "\r\nError: Not Found");
		std::string directoryRoot = infoServer.root + infoServer.location[i].root;
		DIR *dir = opendir(directoryRoot.c_str());
		if (dir == NULL)
			return (response = "HTTP/1.1 404 Not Found\r\n" + date + server + "\r\nError: Not Found");
		closedir(dir);
		
		// check method allowed
		Location infoLocation = infoServer.location[i];
		i = 0;
		while (i < infoLocation.allowMethods.size())
		{
			if (infoResponse.method == infoLocation.allowMethods[i])
				break ;
			i++;
		}
		std::cout << i << " | " << infoLocation.allowMethods.size() << '\n';
		if (i == infoLocation.allowMethods.size())
			return (response = "HTTP/1.1 405 Method Not Allowed\r\n" + date + server + "\r\nError: Method Not Allowed");
		response = "HTTP/1.1 200 OK\r\n" + date + server + "\r\nHello, World!";
		switch (infoLocation.allowMethods[i][0])
		{
			case 'G':
				response = responseGet(infoServer, infoResponse, infoLocation);
				break;
			case 'P':
				response = responsePost(infoServer, infoResponse, infoLocation);
				break;
			case 'D':
				response = responseDelete(infoServer, infoResponse, infoLocation);
				break;
		}
		
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

	}
	return (response);
}
