/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 18:36:52 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 18:38:03 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ARequest.hpp"
#include "VirtualHost.hpp"
#include <algorithm>

// Construction/Destruction ====================================================
ARequest::ARequest(const InputSocket &IOMessage, const VirtualHost& vhost):
	AMessage(IOMessage.getFd()),
	_vhost(const_cast<VirtualHost &>(vhost)),
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
std::map<std::string, Cookie>					ARequest::_headerToCookies(void)
{
	std::map<std::string, Cookie>	cookies;

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

			std::string	key = elemIt->substr(0, posKV - 1);
			std::string	value = elemIt->substr(posKV + 1);

			cookie.setKeyValue(Cookie::kvPair(key, value));
			cookies[key] = cookie;
		}
	}
	return (cookies);
}
