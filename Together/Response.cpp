/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:44:19 by yachen            #+#    #+#             */
/*   Updated: 2024/07/10 14:20:57 by yachen           ###   ########.fr       */
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

std::cout << "dirRoot: " << dirRoot << "\nfile: " << file << '\n';
    if ((dir = opendir(dirRoot.c_str())) == NULL)
		return code;
	std::cout << "opendir ok\n";
	if (!file.empty())
	{
		std::cout << "readdir ok\n";
    	while ((entry = readdir(dir)) != NULL)
		{
			std::cout << "entry name: " << entry->d_name << '\n';
			
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

// trouve le path de la page d'erreur si le code a un page par defaut configure dans le ficheir .config
std::string	Response::findErrorPage( int code, const Server& config )
{
	std::string	path;
	std::map<int, std::string>::const_iterator	it = config.errorPages.find(code);
	if (it != config.errorPages.end())
		path = it->second;
	return	path;
}

// construit une reponse d'erreur selon les pages d'erreurs configure dans le fichier .config
std::string	Response::buildErrorResponse( int code, const Server& config )
{
	std::string	body = "No Specifique Error Page found";
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

// lit le resultat du script cgi, le stock dans le body et met a jour le code HTTP.
int	Response::readCgiResult( int fd, std::string& body )
{
	const size_t bufferSize = 10;
    char	buffer[bufferSize];
    ssize_t bytesRead;
    body.clear();
	bytesRead = read(fd, buffer, bufferSize);
    while (bytesRead > 0)
	{
        body.append(buffer, bytesRead);
		bytesRead = read(fd, buffer, bufferSize);
	}
    if (bytesRead == -1)
        return 500;
    return 200;
}

// cree un processus enfant pour executer le script externe, mettre a jour body le code HTTP.
int	Response::executeCgi( std::string path, std::string& body )
{
	int	code = 200;
	int	pipefd[2];
	if (pipe( pipefd ) == -1)
		return 500;
	pid_t	pid = fork();
	if (pid == -1)
		return 500;
	else if (pid == 0)
	{
		close( STDIN_FILENO );
		close( pipefd[0] );
		dup2( pipefd[1], STDOUT_FILENO );
		close( pipefd[1] );
		const char*	argv[] = { path.c_str(), NULL };
		if (execve( path.c_str(), (char* const* )argv, _env ) == -1)
		{
			close( STDOUT_FILENO );
			exit(1);
		}
	}
	close( pipefd[1] );
	int status;
	waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return 403;
	code = readCgiResult( pipefd[0], body );
	close( pipefd[0] );
	return code;
}

std::string	Response::myGet( Server& config, Location& location, ResponseInfos& infos )
{
	int	response;
	std::string	body;
	std::string	dirRoot = config.root + infos.locationRoot;
	
	if (infos.locationFile.empty())	// si la demande du client se termine par un dossier
	{
		if (!location.index.empty())
		{
			response = checkFileExistence( dirRoot, location.index );
			if (response == 200)
				response = makeBody( dirRoot + location.index, body );
		}
		else if ( location.autoindex == "on" )
		{
			response = checkFileExistence( dirRoot, location.index );
			if (response == 200)
				response = makeListing( dirRoot, body );
		}
		else
			return buildErrorResponse( 404, config );
	}
	else if (infos.locationFile.find( ".sh" ) != std::string::npos )	// si la demande concerne un script
	{
		response = checkFileExistence( dirRoot, infos.locationFile );
		if (response == 200)
			response = executeCgi( dirRoot + infos.locationFile, body );
	}
	else
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

std::string	Response::buildResponse( Request& request )
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
		unsigned long i = 0;			// check root
		while (i < config.location.size() && infos.locationRoot != config.location[i].root)
			i++;
		if (i == config.location.size())
			return buildErrorResponse( 404, config);
		unsigned long j = 0;			// check method allowed
		while (j < config.location[i].allowMethods.size() && infos.method != config.location[i].allowMethods[j])
			j++;
		if (j == config.location[i].allowMethods.size())
			return buildErrorResponse( 405, config);
		
		// if (infos.method == "GET")
		response = myGet( config, config.location[i], infos );
		// else if (infos.method == "POST")
		// 	response = myPost( config, infos );
		// else
		// 	response = myDelete( config, infos );
		
	}
	return (response);
}
