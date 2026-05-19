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
	std::cout << "==> RESPONSE CREATION\nfd: " << this->_fd << " | status: " << this->_status << " | resourceFd: " << this->_resourceFd << std::endl;
}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resourceFd(cpy._resourceFd) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign){ (void) assign; return (*this); }

// Setters =====================================================================
void				Response::setStatus(const uint16_t &status)
{
	// maybe try if already set
	this->_status = status;
	std::cout << "==> STATUS SET\nfd: " << this->_fd << " | status: " << this->_status << " | resourceFd: " << this->_resourceFd << std::endl;
}

void				Response::setResourceFd(const int &resourceFd)
{
	// maybe try if already set
	this->_resourceFd = resourceFd;
	std::cout << "==> RESOURCEFD SET\nfd: " << this->_fd << " | status: " << this->_status << " | resourceFd: " << this->_resourceFd << std::endl;
}

// Getters =====================================================================
const uint16_t		&Response::getStatus(void) const {	return (this->_status); }
const int			&Response::getResourceFd(void) const {	return (this->_resourceFd); }
