/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/17 13:33:42 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

// construit une reponse d'erreur selon les pages d'erreurs configure dans le fichier .config
std::string	Response::buildErrorResponse( int code, const Server& config )
{
	std::string	body;
	std::string path = findErrorPage( code, config );
	if (!path.empty())
	{
		size_t	slash = path.find_last_of( '/', path.size() );
		std::string root = path.substr( 0, slash + 1);
		std::string	file = path.substr( slash + 1, path.size() );
		if (checkFileExistence( root, file ) == 200)
			makeBody( path, body );
	}
	else
	{
		switch (code)
		{
			case 400:
				body = "400 Bad Request\r\n";
				break;
			case 403:
				body = "403 Forbidden\r\n";
				break;
			case 404:
				body = "404 Not Found\r\n";
				break;
			case 405:
				body = "405 Method Not Allowed\r\n";
				break;
			case 408:
				body = "408 Request Timeout\r\n";
				break;
			case 500:
				body = "500 Internal Server Error\r\n";
		}
	}
	std::string	errorResponse = joinHeadersBody( config, body, code );
	return errorResponse;
}

int	Response::deleteFolderRecursive (const std::string& dirPath)
{
	DIR* dir;
	if ((dir = opendir(dirPath.c_str())) == NULL)
		return (403);
	
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
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
	std::string	headersBody = joinHeadersBody(config, body, 200);
	return headersBody;
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
	
	std::string	headersBody = joinHeadersBody( config, body, 200);
	return headersBody;
}

std::string	Response::redirectionHttp( std::pair<int, std::string> redirection )
{	
	std::string	statusLine = "HTTP/1.1 301 Moved Permanently\r\n";
	if (redirection.first == 302)
		std::string	statusLine = "HTTP/1.1 302 Found\r\n";
	return  statusLine + "Location: " + redirection.second + "\r\n"
    		"Content-Length: 0\r\n"
    		"\r\n";
}	

std::string	Response::buildResponse( Request& request, HttpServer& httpServer )
{	
	std::string response;
	ResponseInfos 	infos = request.getResponseInfos();
	Server			config = request.getServerConfig();
	
	std::string host = config.host + ':' + config.listen;
	if (infos.host != host && config.host == "127.0.0.1" && infos.host != "localhost:" + config.listen)
		config = request.getDefaultConfig();
	else if (infos.host != host)
		config = request.getDefaultConfig();
	
	if (request.getCode() != 200)
		return buildErrorResponse( request.getCode(), config);
	else
	{
		unsigned long i = 0;			// verifie si ce root exist dans .config
		while (i < config.location.size() && infos.locationRoot != config.location[i].root)
			i++;
		if (i == config.location.size())
			return buildErrorResponse( 404, config);
		
		if (!config.location[i].redirection.second.empty())	// si il existe une redirection HTTP, priorite avant tout.
			return redirectionHttp( config.location[i].redirection );
			
		unsigned long j = 0;			// verifie si la methode est autorise pour ce root
		while (j < config.location[i].allowMethods.size() && infos.method != config.location[i].allowMethods[j])
			j++;
		if (j == config.location[i].allowMethods.size())
			return buildErrorResponse( 405, config);

		if (infos.method == "DELETE")
			return myDelete(config, infos );
		else if ((!infos.locationFile.empty() && infos.locationFile.find( ".py" ) != std::string::npos)
			|| infos.method == "POST")
		{								//  le client demande a executer un script cgi ou veut post des donnees
			int	code = checkFileExistence( config.root + infos.locationRoot, infos.locationFile );
			if (code != 200)
				return buildErrorResponse( code, config );
			std::string	body;
			char** env = httpServer.createEnvCGI( infos );
			code = httpServer.executeCgi( config.root + infos.locationRoot + infos.locationFile, body, env );
			if (code != 200)
				return buildErrorResponse( code, config );
			response = joinHeadersBody( config, body, 200);
		}
		else if (infos.method == "GET")
			response = myGet( config, config.location[i], infos );
	}
	return (response);
}
