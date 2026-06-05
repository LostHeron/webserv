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
# include "HTMLPageBuilder.hpp"

// Construction/Destruction ====================================================
Response::Response(const int fd):
	AMessage(fd),
	_status(0),
	_resource(std::pair<int, std::string>(-1, std::string("TEST"))),
	_content() {}

// TO DO: add the ability to return existing error page (check Coco)
Response::Response(uint16_t errCode):
	AMessage(-1),
	_status(errCode),
	_resource(std::pair<int, std::string>(errCode, std::string("TEST"))),
	_content(HTMLPageBuilder::errorPage(errCode)) {}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resource(cpy._resource),
	_content(cpy._content) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign){ (void) assign; return (*this); }

// Setters =====================================================================
void									Response::setStatus(const uint16_t &status)
{
	this->_status = status;
}

void		Response::setResource(const int fd, const std::string &path)
{
	this->_resource.first = fd;
	this->_resource.second = path;
}

void									Response::setContent(const std::string &content)
{
	this->_content = content;
}

// Getters =====================================================================
const uint16_t							&Response::getStatus(void) const {	return (this->_status); }
const std::pair<int, std::string>		&Response::getResource(void) const {	return (this->_resource); }
const std::string						&Response::getContent(void) const {	return (this->_content); }
