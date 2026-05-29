/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/28 11:47:14 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ARequest.hpp"
#include "VirtualHost.hpp"
#include <vector>

// Construction/Destruction ====================================================
ARequest::ARequest(const InputSocket &IOMessage, const VirtualHost& vhost):
	AMessage(IOMessage.getFd()),
	_vhost(vhost),
	_method(IOMessage.getMethod()),
	_uri(IOMessage.getUri()),
	_version(IOMessage.getVersion()),
	_header(IOMessage.getHeaders()) {}

ARequest::ARequest(const ARequest &cpy):
	AMessage(cpy._fd),
	_vhost(cpy._vhost),
	_method(cpy._method),
	_uri(cpy._uri),
	_version(cpy._version),
	_header(cpy._header) {}

ARequest::~ARequest(void) {}


// Ops overloading =============================================================
ARequest			&ARequest::operator=(const ARequest &assign){ (void) assign; return (*this); }

// // Setters =====================================================================
// void	ARequest::setType(const std::string &type)		{ this->_type = type; }
// void	ARequest::setHeader(const std::string &header) 	{ this->_header = header; }
// void	ARequest::setBody(const std::string &body) 		{ this->_body = body; }


// // Getters =====================================================================
const std::string										&ARequest::getMethod(void) const { return(this->_method); }
const std::string										&ARequest::getUri(void) const { return(this->_uri); }
const std::string										&ARequest::getVersion(void) const { return(this->_version); }
const std::map<std::string, std::vector<std::string> >	&ARequest::getHeader(void) const { return(this->_header); }
