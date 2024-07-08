/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 16:10:52 by yachen            #+#    #+#             */
/*   Updated: 2024/06/28 16:48:39 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP
#include <map>
#include <string>
#include <vector>

enum tokenType
{
	DIRECTIVE = 1,
	PARAMETER = 2,
	BRACE_OP = 3,
	BRACE_CL = 4
};

typedef struct Token
{
	int	type;
	std::string value;
}	Token;

class	ConfigParser
{
	private:

		char*					_path;
		std::vector<Token*>		_tokenList;
		
		ConfigParser( const ConfigParser& other );
		ConfigParser&	operator=( const ConfigParser& other);
	
		Token*	makeToken( std::string& value );
		void	checkBrace();
		void	checkNumberOfParameter();
		void	checkHostPort();
		void	checkParameterContent( std::vector<Token*>& directive );
		void	checkLocation( std::vector<Token*>& location );
		void	checkServer( std::vector<Token*> server );
		
	public:

		ConfigParser( char* path );
		~ConfigParser();
		
		void					fillTokenList();
		void					analyseTokenList();
		std::vector<Token*>&	getTokenList();
		void					printTokenList();
};

#endif