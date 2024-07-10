/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:16:14 by yachen            #+#    #+#             */
/*   Updated: 2024/07/10 17:31:57 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string Response::getGMTDate()
{
	time_t		currentTime;
	struct tm*	timeInfos;
	const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time( &currentTime );
	timeInfos = gmtime( &currentTime );
	std::stringstream	ss;
	
    ss  << "Date: "
		<< days[timeInfos->tm_wday] << ", "
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_mday << " "
    	<< months[timeInfos->tm_mon] << " "
    	<< (timeInfos->tm_year + 1900) << " "
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_hour << ":"
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_min << ":"
    	<< std::setfill('0') << std::setw(2) << timeInfos->tm_sec
    	<< " GMT\r\n";
	return ss.str();
}

std::string	Response::joinHeadersBody( const Server& config, std::string& body )
{
	std::stringstream	ss;
	ss << body.length();
	std::string	bodyLength = ss.str();
	std::string	headersBody = 	getGMTDate() + "Server: " + config.serverName + "\r\n"
								+ "Content-Type: text/html; charset=UTF-8\r\n"
								+ "Content-Length: " + bodyLength + "\r\n\r\n"
								+ body;
	return headersBody;
}

// verifie si le fichier ou le dossier demande existe
int	Response::checkFileExistence( std::string dirRoot, std::string& file )
{
	DIR* dir;
    struct dirent* entry;
	int	code = 404;

    if ((dir = opendir(dirRoot.c_str())) == NULL)
		return code;
	if (!file.empty())
	{
    	while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name == file)
				code = 200;
		}
	}
	else
		code = 200;
	closedir(dir);
	return code;
}

// lit le contenu du fichier et le stock dans body
int	Response::makeBody( std::string path, std::string& body )
{
	std::ifstream fd(path.c_str());
	if (!fd)
		return (403);
	std::string line;
	body.clear();
	while (std::getline(fd, line))
		body += line;
	return (200);
}

// retourne un listing des sous repertoires et fichier du dossier passe en parametre
int Response::makeListing(const std::string& dirRoot, std::string& body)
{
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(dirRoot.c_str())) == NULL)
		return 500;

    body = "<html><body><h1>Directory Listing</h1><ul>";
    while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
		    continue;
		body += "<li><a href=\"" + name + "\">" + name + "</a></li>";
    }
	closedir(dir);
	body += "</ul></body></html>";
	return 200;
}

// trouve le path de la page d'erreur si le code a un page par defaut configure dans le ficheir .config
std::string	Response::findErrorPage( int code, const Server& config )
{
	std::string	path;
	std::map<int, std::string>::const_iterator	it = config.errorPages.find(code);
	if (it != config.errorPages.end())
		path = it->second;
	return	path;
}
