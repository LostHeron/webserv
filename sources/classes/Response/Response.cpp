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
Response::Response(const int fd):
	AMessage(fd),
	_status(0),
	_resourceFd(-1),
	_content() {}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resourceFd(cpy._resourceFd),
	_content(cpy._content) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign){ (void) assign; return (*this); }

// Setters =====================================================================
void				Response::setStatus(const uint16_t &status)
{
	this->_status = status;
}

void				Response::setResourceFd(const int &resourceFd)
{
	this->_resourceFd = resourceFd;
}

void				Response::setContent(const std::string &content)
{
	this->_content = content;
}

// Getters =====================================================================
const uint16_t		&Response::getStatus(void) const {	return (this->_status); }
const int			&Response::getResourceFd(void) const {	return (this->_resourceFd); }
const std::string	&Response::getContent(void) const {	return (this->_content); }
