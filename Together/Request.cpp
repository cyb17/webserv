/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:36:30 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/07/06 17:58:05 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() {}

Request::Request( Server& configServer ) : _configServer( configServer ), _code( 200 ), _step(firstLine)
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
	std::cout << "* path = " << _infos.path << "\n";
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

std::string getGMTDate()
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

std::string	Request::buildResponse( std::string& requestLine )
{
	if (parseRequest(requestLine) != complete)
		return ("not complete");
	printInfos();
	std::string response;
	std::string date = "Date: " + getGMTDate() + "\r\n";
	std::string server = "Server: " + _configServer.serverName + "\r\n";
	// if (_code == 400)
		response = "HTTP/1.1 200 OK\r\n" + date + server + "Content-Type: text/plain\r\nContent-Length: 13\r\n\r\nbody...";
	std::cout << response << '\n';
	std::string response2;
	response2 =	"HTTP/1.1 200 OK\r\n"
				"Date: teste\n"
				"Server: \r\n"
           		"Content-Type: text/plain\r\n"
           		"Content-Length: 13\r\n"
           		"\r\n"
           		"Hello, World!";
	std::cout << response2 << '\n';
	return response;
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
	if (lineInfo.size() != 3
		|| (lineInfo[0] != "GET" && lineInfo[0] != "POST" && lineInfo[0] != "DELETE")
		|| lineInfo[2] != "HTTP/1.1")
	{
		_code = 400;
		return false;
	}
	_infos.method = lineInfo[0];
	_infos.path = lineInfo[1];
	_infos.version = lineInfo[2];
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
Step			Request::getStep() { return (_step); }
time_t			Request::getStartTime() { return (_startTime); }
ResponseInfos	Request::getResponseInfos() { return (_infos); }


// Request::Request() : _socket(-1), _step(0), _line(""), _responseCode(0) {}
	
// Request::Request(int socket) : _socket(socket), _step(0),_line(""),  _responseCode(0) {}

// Request::~Request() {} 

// int Request::parsing()
// {
// 	char buffer[1];
// 	int len = 1;

// 	while (len > 0)
// 	{
// 		len = recv(_socket, buffer, 1, 0);
// 		if (len == -1)
// 		{
// 			if (errno != EAGAIN || errno != EWOULDBLOCK)
// 			{
// 				printMsgError("recv");
// 				return (_responseCode = 500);
// 			}
// 		}
// 		buffer[len] = '\0';
// 		_line += buffer;
// 		if (_step == 0 && _line != "\r\n")
// 				_step = 1;
// 		if (_step > 0 && buffer[0] == '\n') 
// 		{
// 			if (init() > 0)
// 				break;
// 			else
// 				_line = "";
// 		}
// 	}
// 	return (_responseCode);
// }

// int Request::init()
// {
// 	if (_firstLine.empty() && _line != "\r\n" && _step == 1)
// 		return (setFirstLine());
// 	if (_step == 2)
// 		return (setHeaders());
// 	// if (_step == 3)
// 	// {
// 	// 	if (_line == "\r")
// 	// 		return (_responseCode = 200);
// 	// 	else 
// 	// 	{
// 	// 		_body.push_back(_line);
// 	// 		return (_responseCode = 0);
// 	// 	}
// 	// }
// 	return (0);
// }

// //SETTERS

// int Request::setFirstLine()
// {
// 	std::istringstream iss(_line);
// 	std::string	word;
// 	std::string firstline[3] = {"method", "url", "http"};
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		if (iss >> word)
// 			_firstLine.insert(std::make_pair(firstline[i], word));
// 	}
// 	if (iss >> word || _firstLine.size() != 3)
// 		return (_responseCode = 400);
// 	if (checkFirstLine() == false)
// 		return (_responseCode);
// 	_step++;
// 	return (0);
// }

// int Request::setHeaders()
// {
// 	if (_line == "\r\n")
// 	{
// 		if (_headers.empty())
// 			return(_responseCode = 400);
// 		else
// 			return (_step++, _responseCode = 200);
// 	}
// 	std::istringstream iss(_line);
// 	std::string header;
// 	std::string word;
// 	std::string value;
		
// 	iss >> header;
// 	while (iss >> word)
// 	{
// 		value += word;
// 		value += " ";
// 	}
// 	if (header == "Host:" && value.empty())
// 		return (_responseCode = 400);
// 	_headers.insert(std::make_pair(header, value));
// 	return (0);
// }

// // CHECK FUNCTIONS

// bool Request::checkFirstLine()
// {
// 	std::map<std::string,std::string>::iterator it;
	
// 	//Check method
// 	it = _firstLine.find("method");
// 	std::string method[3] = {"GET", "POST", "DELETE"};
// 	if (it != _firstLine.end())
// 	{
// 		int i = 0;
// 		while (i < 3)
// 		{
// 			if (it->second == method[i++])
// 				break;
// 		}
// 		if (i == 3)
// 			return (_responseCode = 400, false);
// 	}
// 	//Check version http
// 	it = _firstLine.find("http");
// 	if (it->second != "HTTP/1.1")
// 		return (_responseCode = 400, false);
// 	//Check url
// 	it = _firstLine.find("url");
// 	if (it != _firstLine.end())
// 	{
// 		std::string path = it->second;
// 		std::ifstream file(path.c_str());
		
// 		if (!file)
// 			return (_responseCode = 404, false);
// 		file.close();
// 	}
// 	return (true);
// }

// bool Request::checkHeaders()
// {
// 	std::map<std::string,std::string>::iterator it;
	
//     for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
// 	{
//     	if (it->first == "Host:")
// 			return (true);
// 	}
// 	return (false);
// }

// //GETTERS

// int	Request::getResponseCode() { return (_responseCode); }

// //PRINT FUNCTIONS

// void Request::printFirstLine() const
// {
//     std::map<std::string, std::string>::const_iterator it;
	
//     for (it = _firstLine.begin(); it != _firstLine.end(); ++it)
//         std::cout << it->first << ": " << it->second << "\n";
// }

// void Request::printHeaders() const
// {
//     std::map<std::string, std::string>::const_iterator it;
	
//     for (it = _headers.begin(); it != _headers.end(); ++it)
//         std::cout << it->first << " " << it->second << "\n";
// }

// void Request::printMsgError(const char *err)
// {
// 	std::cerr << "ERROR : " << err << " => " << strerror(errno) << "\n";
// }