/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teste.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachen <yachen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:02:18 by yachen            #+#    #+#             */
/*   Updated: 2024/07/06 14:31:26 by yachen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

int main()
{
	time_t		currentTime;
	struct tm*	timeInfos;

	const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	time( &currentTime );
	// timeInfos = gmtime( &currentTime );
	timeInfos = localtime( &currentTime );
	std::stringstream	ss;
	
	ss << "Date: "
       << days[timeInfos->tm_wday] << ", "
       << std::setfill('0') << std::setw(2) << timeInfos->tm_mday << " "
       << months[timeInfos->tm_mon] << " "
       << (timeInfos->tm_year + 1900) << " "
       << std::setfill('0') << std::setw(2) << timeInfos->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << timeInfos->tm_min << ":"
       << std::setfill('0') << std::setw(2) << timeInfos->tm_sec
       << " GMT";
	std::cout << ss.str();

}