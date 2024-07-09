/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/09 13:52:10 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( char** env ) : _env( env ) {}
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
// int	makeBody( std::string path, std::string& body )

// int	makeListing( std::string dirRoot, std::string& body )

void	ReadCgiResult( int pipe, int& code, std::string& body )
{
	const size_t bufferSize = 10;
    char	buffer[bufferSize];
    ssize_t bytesRead;
    body.clear();
	bytesRead = read(pipe, buffer, bufferSize);
    while (bytesRead > 0)
	{
        body.append(buffer, bytesRead);
		bytesRead = read(pipe, buffer, bufferSize);
	}
    if (bytesRead == -1)
    {
        code = 500;
        body = "Internal Server Error";
    }
    else
        code = 200;
}

int	Response::executeCgi( std::string path,std::string& body )
{
	int	code = 200;
	int	pipefd[2];
	if (pipe( pipefd ) == -1)
		return body = "Internal Server Error", 500;
	pid_t	pid = fork();
	if (pid == -1)
		return body = "Internal Server Error", 500;
	else if (pid == 0)
	{
		close( STDIN_FILENO );
		close( pipefd[0] );
		dup2( pipefd[1], STDOUT_FILENO );
		close( pipefd[1] );
		const char*	argv[] = { path.c_str(), NULL };
		// std::cerr << path.c_str() << '\n';
		if (execve( path.c_str(), (char* const* )argv, _env ) == -1)
		{
			close( STDOUT_FILENO );
			exit(1);
		}
	}
	int status;
	waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return body = "Internal Server Error", 500;
	close( pipefd[1] );
	readCgiResult( pipefd[0], code, body );
	close( pipefd[0] );
	return code;
}

std::string	Response::myGet( Server& config, ResponseInfos& infos )
{
	std::string	dirRoot = config.root + infos.locationRoot;
	int	response;
	std::string	body;
	// if (infos.locationFile.empty())
	// {
	// 	if (!config.index.empty())
	// 		response = makeBody( dirRoot + config.index, body );
	// 	else if ( config.autoindex == "on" )
	// 		response = makeListing( dir, body );
	// 	else
	// 		return build( 404, config.serverName, "Error: Not Found" );
	// }
	if (infos.locationFile.find( ".sh" )!= std::string::npos )
		response = executeCgi( dirRoot + infos.locationFile, body );
	else
		response = 200;
	// 	response =  makeBody( dirRoot + infos.locationFile, body );
	return build( response, config.serverName, body );
}

std::string	Response::buildResponse( Request& request )
{	
	ResponseInfos 	infos = request.getResponseInfos();
	Server			config = request.getServerConfig();
	std::string response;
	
	response = build( 200, config.serverName, "Hello World" );
	
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
		
		// response = build( 200, config.serverName, "Hello World" );
	}
	return (response);
}
