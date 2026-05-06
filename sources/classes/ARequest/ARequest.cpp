/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/10 17:37:33 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ARequest.hpp"

// Construction/Destruction ====================================================
// ARequest::ARequest(void):
// 	AMessage(),
// 	_method(""),
// 	_uri(""),
// 	_version(""),
// 	_header(0),
// 	_body(0) {}
//
ARequest::ARequest(const IOFd &IOMessage):
	AMessage(IOMessage.getFd()),
	_method(IOMessage.getMethod()),
	_uri(IOMessage.getUri()),
	_version(IOMessage.getVersion()),
	_header(IOMessage.getHeader()),
	_body(IOMessage.getBody()) {}

ARequest::ARequest(const ARequest &cpy):
	AMessage(cpy._fd),
	_method(cpy._method),
	_uri(cpy._uri),
	_version(cpy._version),
	_header(cpy._header),
	_body(cpy._body) {}

ARequest::~ARequest(void) {}


// Ops overloading =============================================================
ARequest			&ARequest::operator=(const ARequest &assign){ (void) assign; return (*this); }

// // Setters =====================================================================
// void	ARequest::setType(const std::string &type)		{ this->_type = type; }
// void	ARequest::setHeader(const std::string &header) 	{ this->_header = header; }
// void	ARequest::setBody(const std::string &body) 		{ this->_body = body; }
//
// // Getters =====================================================================
// const std::string	&ARequest::getType(void) const		{ return(this->_type); }
// const std::string	&ARequest::getHeader(void) const	{ return(this->_header); }
// const std::string	&ARequest::getBody(void) const		{ return(this->_body); }
