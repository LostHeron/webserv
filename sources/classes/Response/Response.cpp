/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/10 17:37:33 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"

// Construction/Destruction ====================================================
Response::Response(const int &fd):
	AMessage(fd),
	_status(0),
	_resourceFd(-1)
{
	std::cout << "fd: " << this->_fd << " | status: " << this->_status << " | resourceFd: " << this->_resourceFd << std::endl;
}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resourceFd(cpy._resourceFd) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign){ (void) assign; return (*this); }

// Setters =====================================================================
void				Response::setResourceFd(const int &resourceFd)
{
	// maybe try if already set
	this->_resourceFd = resourceFd;
}

// Getters =====================================================================
