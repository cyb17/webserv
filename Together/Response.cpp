/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/10 17:59:37 by yachen           ###   ########.fr       */
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

std::string	Response::buildResponse( Request& request, HttpServer& httpServer )
{	
	ResponseInfos 	infos = request.getResponseInfos();
	Server			config = request.getServerConfig();
	if (infos.host != config.serverName)
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
	
		if (infos.method == "DELETE")
			response = "HTTP/1.1 200 OK\r\nDELETE"
		else if (!infos.locationFile.empty() && infos.locationFile.find( ".sh" ))	//  le client demande a executer un script cgi
		{
			int	code = checkFileExistence( config.root + infos.locationRoot, infos.locationFile );
			if (code == 200)
			{
				std::string	body;
				code = httpServer.executeCgi( config.root + infos.locationRoot + infos.locationFile, body );
				if (code == 200)
					response = "HTTP/1.1 200 OK\r\n" + joinHeadersBody( config, body );
				else
					response = buildErrorResponse( code, config );
			}
			else
				response = buildErrorResponse( code, config );
		}
		else if (infos.method == "GET")
			response = myGet( config, config.location[i], infos );
		else if (infos.method == "POST")
			response = "HTTP/1.1 200 OK\r\nPOST";//myPost( config, infos );
	}
	return (response);
}
