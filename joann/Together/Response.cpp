/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/12 14:05:43 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( char** env ) : _env( env ) {}

Response::~Response() {}

// construit une reponse d'erreur selon les pages d'erreurs configure dans le fichier .config
std::string	Response::buildErrorResponse( int code, const Server& config )
{
	std::string	body = "No Specific Error Page found";
	std::string path = findErrorPage( code, config );
	if (!path.empty())
	{
		size_t	slash = path.find_last_of( '/', path.size() );
		std::string root = path.substr( 0, slash + 1);
		std::string	file = path.substr( slash + 1, path.size() );
		if (checkFileExistence( root, file ) == 200)
			makeBody( path, body );
	}
	std::string	headersBody = joinHeadersBody( config, body );
	switch (code)
	{
		case 400:
			return "HTTP/1.1 400 Bad Request\r\n" + headersBody;
		case 403:
			return "HTTP/1.1 403 Forbidden\r\n" + headersBody;
		case 404:
			return "HTTP/1.1 404 Not Found\r\n" + headersBody;
		case 405:
			return "HTTP/1.1 405 Method Not Allowed\r\n" + headersBody;
		case 408:
			return "HTTP/1.1 408 Request Timeout\r\n" + headersBody;
		case 500:
			return "HTTP/1.1 500 Internal Server Error\r\n" + headersBody;
	}
	return "HTTP/1.1 200 OK\r\n" + headersBody;
}

int	Response::deleteFolderRecursive (const std::string& dirPath)
{
	DIR* dir;
	if ((dir = opendir(dirPath.c_str())) == NULL)
		return (403);
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::cout << "ok boucle\n";
        std::string tmp(entry->d_name);
		if (tmp.compare(".") == 0 || tmp.compare("..") == 0)
			continue ;
		tmp = dirPath + "/" + entry->d_name;
		struct stat buff;
		if (stat(tmp.c_str(), &buff) == 0)
		{
			if (S_ISDIR(buff.st_mode))
			{
				int code = deleteFolderRecursive(tmp);
				std::cout << "ok boucle code: "<< code << '\n';
				if (code != 200)
					return (closedir(dir), code);
			}
			else
			{
				if (remove(tmp.c_str()) != 0)
					return (closedir(dir), 500);
			}
		}
	}
	closedir(dir);
	if (rmdir(dirPath.c_str()) == -1)
		return (500);
	return (200);
}

std::string Response::myDelete(Server& config, ResponseInfos& infos)
{
	std::string	body;
	std::string	path = config.root + infos.locationRoot + infos.locationFile;
	
	struct stat buff;
	if (stat(path.c_str(), &buff) == 0)
	{
		if (S_ISDIR(buff.st_mode))
		{
			int code = deleteFolderRecursive(path);
			if (code != 200)
				return (buildErrorResponse(code, config));
			body = "Directory deleted.\r\n";
		}
		else
		{
			if (remove(path.c_str()) != 0)
				return (buildErrorResponse(500, config));
			body = "File deleted.\r\n";
		}
	}
	else 
		return (buildErrorResponse(500, config));
	std::string	headersBody = joinHeadersBody(config, body);
	return "HTTP/1.1 200 OK\r\n" + headersBody;
}

// recupere une ressource demande par la requete HTTP
std::string	Response::myGet( Server& config, Location& location, ResponseInfos& infos )
{
	int	response;
	std::string	body;
	std::string	dirRoot = config.root + infos.locationRoot;
	
	if (infos.locationFile.empty())	// la ressource demande est un dossier
	{
		response = checkFileExistence( dirRoot, location.index );
		if (response == 200 && !location.index.empty())
				response = makeBody( dirRoot + location.index, body );
		else if (response == 200 && location.autoindex == "on" )
				response = makeListing( dirRoot, body );
	}
	else							// la resource demande est un fichier
	{
		response = checkFileExistence( dirRoot, infos.locationFile );
		if (response == 200)
			response =  makeBody( dirRoot + infos.locationFile, body );
	}
	if (response != 200)
		return buildErrorResponse( response, config );
	
	std::string	headersBody = joinHeadersBody( config, body );
	return "HTTP/1.1 200 OK\r\n" + headersBody;
}

// std::string	Response::redirectionHttp( std::pair<int, std::string> redirection, const Server& config )
// {
// 	int	code;
// 	std::string	body;
// 	std::string	response;
// 	std::string	path = redirection.second;
// 	size_t	slash = path.find_last_of( '/', path.size() );
// 	std::string root = path.substr( 0, slash + 1);
// 	std::string	file = path.substr( slash + 1, path.size() );

// 	code = checkFileExistence( root, file );
// 	if (code == 200 && file.empty())
// 	{
// 		DIR* dir;
//     	struct dirent* entry;
// 		dir = opendir( root.c_str() );
// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			std::string	tmp( entry->d_name );
// 			if (tmp == "index.html" || tmp == "index.htm")
// 				file = tmp;
// 		}
// 		if (file.empty())
// 			code = makeListing( root, body );
// 		else
// 			code = makeBody( root + file, body );
// 	}
// 	else if (code == 200)
// 		code = makeBody( root + file, body );
// 	if (code != 200)
// 		return buildErrorResponse( code, config );
// 	std::string	headersBody = joinHeadersBody( config, body );
// 	std::string	statusLine = "HTTP/1.1 301 Moved Permanently\r\n";
// 	if (redirection.first == 302)
// 		std::string	statusLine = "HTTP/1.1 302 Found\r\n";
// 	return statusLine + headersBody;
// }

// std::string	Response::redirectionHttp( std::pair<int, std::string> redirection, const Server& config )
// {
// 	std::string	path = redirection.second;
// 	size_t	slash = path.find_last_of( '/', path.size() );
// 	std::string root = path.substr( 0, slash + 1);
// 	std::string	file = path.substr( slash + 1, path.size() );
// }

std::string	Response::buildResponse( Request& request, HttpServer& httpServer )
{	
	ResponseInfos 	infos = request.getResponseInfos();
	Server			config = request.getServerConfig();
	
	std::string host = config.host + ':' + config.listen;
	if (config.host == "127.0.0.1" && infos.host != host && infos.host != "localhost:" + config.listen)
		config = request.getDefaultConfig();
	else if (infos.host != host)
		config = request.getDefaultConfig();
		
	std::string response;
	if (request.getCode() == 400)
		return buildErrorResponse( 400, config);
	else
	{
		unsigned long i = 0;			// verifie si ce root exist dans .config
		while (i < config.location.size() && infos.locationRoot != config.location[i].root)
			i++;
		if (i == config.location.size())
			return buildErrorResponse( 404, config);
		unsigned long j = 0;			// verifie si la methode est autorise pour ce root
		while (j < config.location[i].allowMethods.size() && infos.method != config.location[i].allowMethods[j])
			j++;
		if (j == config.location[i].allowMethods.size())
			return buildErrorResponse( 405, config);
		
		if (!config.location[i].redirection.second.empty())	// si il existe une redirection HTTP, elle sera traite en priorite.
		{
			std::cout << "rentre dans la redirection\n";
			return  "HTTP/1.1 301 Moved Permanently\r\n"
        			"Location: http://localhost\r\n"
        			"Content-Length: 0\r\n"
        			"Connection: close\r\n"
        			"\r\n";
		}
			// return redirectionHttp( config.location[i].redirection, config );
		
		if (infos.method == "DELETE")
			return myDelete(config, infos );
		else if (!infos.locationFile.empty() && infos.locationFile.find( ".sh" ))	//  le client demande a executer un script cgi
		{
			int	code = checkFileExistence( config.root + infos.locationRoot, infos.locationFile );
			if (code != 200)
				return buildErrorResponse( code, config );
			std::string	body;
			code = httpServer.executeCgi( config.root + infos.locationRoot + infos.locationFile, body );
			if (code != 200)
				return buildErrorResponse( code, config );
			response = "HTTP/1.1 200 OK\r\n" + joinHeadersBody( config, body );
		}
		else if (infos.method == "GET")
			response = myGet( config, config.location[i], infos );
		else if (infos.method == "POST")
			response = "HTTP/1.1 200 OK\r\nPOST";
	}
	return (response);
}
