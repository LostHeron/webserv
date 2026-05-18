/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:39:13 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETReq.hpp"

// Constructors/Destructor =====================================================
// GETReq::GETReq(void):
// 	ARequest(type, header, body) {}

GETReq::GETReq(const ARequest &cpy):
	ARequest(cpy) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
Response	GETReq::execute(void)
{
	Response resp(this->_fd);

	const std::string path = TEMP_ROOT + this->_uri;
	std::cout	<< "URI to fetch: " << this->_uri 
				<< " for full path: " << path
				<< std::endl;
	
	// check perms

	// if dir, return index page, else:
	
	// try open
	int	resourceFd = open(path.c_str(), O_RDONLY);
	if (resourceFd < 0)
	{
		std::cout << "ERROR APPENDS" << std::endl;
		return (resp);
	}
	int rv = 0;
	char buf[1024 + 1];
	do
	{
		buf[rv] = '\0';
		std::cout << buf;
		rv = read(resourceFd, buf, 1024);
	} while (rv);




	// return resourceFd

	return (resp);
}
