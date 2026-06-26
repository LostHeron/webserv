/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/22 16:46:31 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ARequest.hpp"
#include "VirtualHost.hpp"
#include <algorithm>

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

// Setters =====================================================================


// Getters =====================================================================
const std::string										&ARequest::getMethod(void)	const	{ return(this->_method); }
std::string												ARequest::getUri(void) 				{ return(this->_uri); }
const std::string										&ARequest::getVersion(void) const 	{ return(this->_version); }
const std::map<std::string, std::vector<std::string> >	&ARequest::getHeader(void)	const 	{ return(this->_header); }

// Member Functions ============================================================
std::vector<Cookie>					ARequest::_headerToCookie(void)
{
	std::vector<Cookie>	cookies;

	if (this->_header.count("cookie") == 0)
		return (cookies);

	std::vector<std::string>::iterator	headersIt;

	for (headersIt = this->_header["cookie"].begin(); headersIt != this->_header["cookie"].end(); ++headersIt)
	{

		std::vector<std::string>	splitCookies;
		std::string					elem(*headersIt);


		size_t	posElem = elem.find_first_of("; ");
		while (posElem != std::string::npos)
		{
			posElem = elem.find_first_of("; ");
			splitCookies.push_back(elem.substr(0, posElem));
			elem = elem.substr(posElem + 2);
		}

		std::vector<std::string>::iterator	elemIt;
		for (elemIt = splitCookies.begin(); elemIt != splitCookies.end(); ++elemIt)
		{
			size_t		posKV = elemIt->find('=');
			Cookie		cookie;

			cookie.setKeyValue(Cookie::kvPair(elemIt->substr(0, posKV - 1), elemIt->substr(posKV + 1)));
			cookies.push_back(cookie);
		}

	}
	return (cookies);
}

// void				ARequest::_updateCookies(const std::vector<Cookie> &request, std::vector<Cookie> &response) const
// {
// 	std::vector<Cookie>::const_iterator	it;
//
// 	for (it = request.begin(); it != request.end(); ++it)
// 	{
// 		// ID
// 		if (expired)
// 		{
// 			// if expired replace
//
// 		}
// 		else
// 		{
// 			response.setKeyValue(Cookie::kvPair(Cookie::permanentCookies[SESSION], Cookie::defineSessionId(void)));
// 		}
// 	}
// }
