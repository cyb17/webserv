/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jp-de-to <jp-de-to@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:35:27 by jp-de-to          #+#    #+#             */
/*   Updated: 2024/06/26 16:37:53 by jp-de-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <sys/socket.h>
# include <string>
# include <map>
# include <vector>

class Request
{
	private:
		int								_socket;
		bool							_valid;
		std::vector<std::string>		_msg;
		std::map<char, std::string> 	_firstLine;
		std::map<char, std::string>		_headers;
		std::string						_body;	
		Request();
	
	public:
		Request(int socket);
		~Request();

		//SETTERS
		void setMsg();
		void init();
		
};

#endif