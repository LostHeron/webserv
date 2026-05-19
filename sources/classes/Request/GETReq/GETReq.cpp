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

// tempp!!!!
void send_file(int fd, int client)
{
	int rv = 0;
	char buf[1024 + 1];
	do
	{
		buf[rv] = '\0';
		write(client, buf, rv);
		rv = read(fd, buf, 1024);
	} while (rv);
}
// tempp!!!!

Response	GETReq::execute(void)
{
	Response resp(this->_fd);

	const std::string path = TEMP_ROOT + this->_uri;
	std::cout	<< "URI to fetch: " << this->_uri 
				<< " for full path: " << path
				<< std::endl;
	
	// check perms = config_file

	// check dir or file, return index page
	
	// try open: existence/chmod
	int	resourceFd = open(path.c_str(), O_RDONLY);
	if (resourceFd < 0)
	{
		std::cout << "OPEN: ERROR APPENDS" << std::endl;
		return (resp);
	}

	// metadata settings
	resp.setStatus(200);
	resp.setResourceFd(resourceFd);

	// DEBUG
	send_file(resourceFd, this->_fd);
	// DEBUG



	return (resp);
}
