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
ARequest::ARequest(void):
	_type(0),
	_header(0),
	_body(0) {}

ARequest::ARequest(const uint8_t &type, const std::string &header, const std::string &body):
	_type(type),
	_header(header),
	_body(body) {}

ARequest::ARequest(const ARequest &cpy):
	_type(cpy._type),
	_header(cpy._header),
	_body(cpy._body) {}

ARequest::~ARequest(void) {}


// Ops overloading =============================================================
ARequest			&ARequest::operator=(const ARequest &assign){ (void) assign; }


// Getters =====================================================================
const uint8_t		&ARequest::getType(void) const		{ return(this->_type) };
const std::string	&ARequest::getHeader(void) const	{ return(this->_header) };
const std::string	&ARequest::getBody(void) const		{ return(this->_body) };

#endif
