/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:40:46 by yachen            #+#    #+#             */
/*   Updated: 2024/07/08 13:46:43 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPONSE_HPP
# define RESPONSE_HPP

#include "Request.hpp"

class	Response
{
	private:
		
		
	public:

		Response();
		~Response();

		std::string	buildResponse( Request& request );

};

#endif