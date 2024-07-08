/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/08 17:11:14 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() {}
Response::~Response() {}

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

std::string	Response::build( int code, std::string& serverName, std::string body )
{
	std::string date = getGMTDate();
	std::string server = "Server: " + serverName + "\r\n";
	switch (code)
	{
		case 400:
			return "HTTP/1.1 400 Bad Request\r\n" + date + server + "\r\n" + body;
		case 404:
			return "HTTP/1.1 404 Not Found\r\n" + date + server + "\r\n" + body;
		case 405:
			return "HTTP/1.1 405 Method Not Allowed\r\n" + date + server + "\r\n" + body;
	}
	return "HTTP/1.1 200 OK\r\n" + date + server + "\r\n" + body;
}

// retourne une reponse http avec comme body le contenu du fichier lu
int	makeBody( std::string path, std::string& body )
{
	std::ifstream file(path.c_str());
	if (!file)
	{
		body = "Forbidden\r\n";
		return (403);
	}
	std::string line;
	while (std::getline(file, line))
		body += line;
	return (200);
}

// int	makeListing( std::string dirRoot, std::string& body )

// int	executeCgi( std::string path,std::string& body )

std::string	myGet( Server& config, ResponseInfos& infos )
{
	std::string	dirRoot = config.root + infos.locationRoot;
	int	response;
	std::string	body;
	if (infos.locationFile.empty())
	{
		if (!config.index.empty())
			response = makeBody( dirRoot + config.index, body );
		else if ( config.autoindex == "on" )
			response = makeListing( dir, body );
		else
			return build( 404, config.serverName, "Error: Not Found" );
	}
	else if (infos.locationFile.find( ".py" != std::string::npos ))
		response = executeCgi( dir + infos.locationFile, body );
	else
		response =  makeBody( dir + infos.locationFile, body );
	return build( response, config.serverName, body );
}

std::string	Response::buildResponse( Request& request )
{	
	ResponseInfos 	infos = request.getResponseInfos();
	Server			config = request.getServerConfig();
	std::string response;
	if (infos.host != config.serverName)
		config = request.getDefaultConfig();
	if (request.getCode() == 400)
		return build( 400, config.serverName, "Error: Bad Request" );
	else
	{
		unsigned long i = 0;			// check root
		while (i < config.location.size() && infos.locationRoot != config.location[i].root)
			i++;
		if (i == config.location.size())
			return build( 404, config.serverName, "Error: Not Found" );
		
		unsigned long j = 0;			// check method allowed
		while (j < config.location[i].allowMethods.size() && infos.method != config.location[i].allowMethods[j])
			j++;
		if (j == config.location[i].allowMethods.size())
			return build( 405, config.serverName, "Error: Method Not Allowed" );		
		
		if (infos.method == "GET")
			response = myGet( config, infos );
		// else if (infos.method == "POST")
		// 	response = myPost( config, infos );
		// else
		// 	response = myDelete( config, infos );
		
		response = build( 200, config.serverName, "Hello World" );
	}
	return (response);
}
