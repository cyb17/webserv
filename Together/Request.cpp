/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/08 13:33:29 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {}

Request::Request( Server& configServer, Server& defaultServer ) : _defaultConfigServer(defaultServer), _configServer( configServer ), _code( 200 ), _step(firstLine)
{
	time(&_startTime);
	_infos.bodyLengthRequest = -1;
	_infos.bodyLen = 0;
}

Request::~Request() {}

void	Request::printInfos()
{
	std::cout << "REQUEST INFOS\n\n";
	std::cout << "Code = " << _code << "\n";
	std::cout << "* method = " << _infos.method << "\n";
	std::cout << "* locationRoot = " << _infos.locationRoot << "\n";
	std::cout << "* locationFile = " << _infos.locationFile << "\n";
	std::cout << "* version = " << _infos.version << "\n";
	std::cout << "* host = " << _infos.host << "\n";
	if (_infos.body.size() > 0)
	for (unsigned long i = 0; i << _infos.body.size(); ++i)
	{
		if (i == 0)
			std::cout << "* body = " << _infos.body[i] << "\n";
		else
			std::cout << "         " << _infos.body[i] << "\n";
	}
	std::cout << "\n\n";
}

void	Request::printServer(Server &info)
{
	std::cout << "SERVER INFOS\n\n";
	std::cout << "* Server name: " << info.serverName << "\n";
	std::cout << "* Host: " << info.host << "\n";
	std::cout << "* Listen: " << info.listen << "\n";
}

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

// std::string	Request::responseGet(Server& infoServer, ResponseInfos& infoResponse, Location& infoLocation)
// {

// }

std::string	Request::buildResponse( std::string& requestLine )
{
	// if (parseRequest(requestLine) != complete)
	// 	return ("not complete");
	// printInfos();
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

std::string	Request::responseGet(Server& infoServer, ResponseInfos& infoResponse, Location& infoLocation)
{
	// dossier
	if (infoResponse.locationFile.empty())
	{
		
	}
	// file
}

std::string	Request::responsePost(Server& infoServer, ResponseInfos& infoResponse, Location& infoLocation)
{

}

std::string	Request::responseDelete(Server& infoServer, ResponseInfos& infoResponse, Location& infoLocation)
{

}

Step	Request::parseRequest( std::string& requestLine)
{
	std::istringstream	request(requestLine);
	std::string line;
	
	if (_step == firstLine && std::getline(request, line))
	{
		if (line == "\r")
			return (_step);
		if (isGoodRequestLine( line ) == false)
			return (_step = complete);
		_step = headers;
	}
	if (_step == headers)
	{
		while (std::getline(request,line) && line != "\r")
			_headersTmp.push_back(line);
		if (_headersTmp.empty() && line == "\r")
			return (_code = 400, _step = complete);
		if (line == "\r")
			_step = body;
		if (_step == body)
		{
			if (isGoodHeaders(_headersTmp) == false)
				return (_step = complete);
			if (_infos.method != "POST")
				return (_step = complete);
		}
	}
	if (_step == body)
	{
		if (_infos.bodyLengthRequest <= 0)
			return (_code = 400, _step = complete);
		while (std::getline(request, line))
		{
			_infos.bodyLen += line.size();
			std::string::iterator it = line.end() - 1;
			if (*it == '\r')
				_infos.bodyLen++;
			if (_infos.body.empty() && line == "\r")
				return (_code = 400, _step = complete);
			_infos.body.push_back(line);
			if (_infos.bodyLen > _infos.bodyLengthRequest || _infos.bodyLen > _configServer.clientMaxBodySize)
				return (_code = 400, _step = complete);
			else if (_infos.bodyLen== _infos.bodyLengthRequest)
				return (_step = complete);
		}
	}
	return (_step);
}

bool	Request::isGoodRequestLine( std::string& requestLine)
{
	std::istringstream	iss( requestLine );
	std::vector<std::string>	lineInfo;
	std::string word;
	while ( iss >> word )
		lineInfo.push_back( word );
	size_t	lastSlash = lineInfo[1].find_last_of( '/', lineInfo[1].size() );
	if (lineInfo.size() != 3 || lastSlash == std::string::npos
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE")
		|| lineInfo[2] != "HTTP/1.1")
	{
		_code = 400;
		return false;
	}
	_infos.method = lineInfo[0];
	_infos.version = lineInfo[2];
	_infos.locationRoot = lineInfo[1].substr( 0, lastSlash + 1 );
	_infos.locationFile = lineInfo[1].substr( lastSlash + 1, lineInfo[1].length() );
	return true;
}

bool	Request::isGoodHeaders( std::vector<std::string>& headers )
{
	if (headers.empty())
		return (_code = 400, false);
	int contentType = 0;
	for (unsigned long i = 0; i < headers.size(); ++i)
	{
		std::istringstream iss(headers[i]);
		std::vector<std::string> line;
		std::string word;
		while (iss >> word)
			line.push_back(word);
		if (line[0] == "Host:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			_infos.host = word;
		}
		else if (line[0] == "Content-Type:" && _infos.method == "POST" )
			contentType = 1;
		else if (contentType && line[0] == "Content-Length:")
		{
			if (line.size() != 2)
				return (_code = 400, false);
			for (unsigned long i = 0; i < line[1].size(); ++i)
			{
				if (!isdigit(word[i]) || (line[1].size() == 1 && word[0] == '0'))
					return (_code = 400, false);
			}
			_infos.bodyLengthRequest = atoi(word.c_str());
		}
	}
	if (_infos.host.empty())
		return (_code = 400, false);
	return (true);
}

int				Request::getCode() { return (_code); }
time_t			Request::getStartTime() { return (_startTime); }
ResponseInfos	Request::getResponseInfos() { return (_infos); }
Server 			Request::getDefaultConfig() { return (_defaultConfigServer)};
Server			Request::getServerConfig()  {return (_configServer)};
