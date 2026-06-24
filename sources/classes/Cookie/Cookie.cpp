/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 17:38:03 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/24 12:20:33 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookie.hpp"

// Static attr. initialization =================================================

// Constructors/Destructors ====================================================
Cookie::Cookie(void):
	_keyValue("", ""),
	_domain(""),
	_path(""),
	_maxAge(""),
	_expires(""),
	_HttpOnly(false),
	_secure(false),
	_sameSite(NONE) {}

Cookie::~Cookie(void) {}

// Getters =====================================================================
Cookie::kvPair		Cookie::getKeyValue(void)	{	return (this->_keyValue);	}
std::string			Cookie::getDomain(void)		{	return (this->_domain);		}
std::string			Cookie::getPath(void)		{	return (this->_path);		}
std::string			Cookie::getMaxAge(void)		{	return (this->_maxAge);		}
std::string			Cookie::getExpires(void) 	{	return (this->_expires);	}
bool				Cookie::gethttpOnly(void) 	{	return (this->_HttpOnly);	}
bool				Cookie::getSecure(void) 	{	return (this->_secure);		}
char				Cookie::getSameSite(void) 	{	return (this->_sameSite);	}


// Setters =====================================================================
void			Cookie::setKeyValue(const Cookie::kvPair &kv)
{
	this->_keyValue = kv;
}

void			Cookie::setDomain(const std::string &domain)
{
	this->_domain = domain;
}

void			Cookie::setPath(const std::string &path)
{
	this->_path = path;
}

void			Cookie::setMaxAge(const std::string &age)
{
	this->_maxAge = age;
}

void			Cookie::setExpires(const std::string &date)
{
	this->_expires = date;
}

void			Cookie::setHttpOnly(const bool isHttpOnly)
{
	this->_HttpOnly = isHttpOnly;
}

void			Cookie::setSecure(const bool isSecure)
{
	this->_secure = isSecure;
}

void			Cookie::setSameSite(const char sameSiteFlag)
{
	this->_sameSite = sameSiteFlag;
}


// Member functions ============================================================
std::string	Cookie::cookieToStr(void) const
{
	std::string	cookieHeader;

	cookieHeader += "Set-Cookie: ";
	cookieHeader += _keyValueToStr();
	cookieHeader += _domainToStr();
	cookieHeader += _pathToStr();
	cookieHeader += _maxAgeToStr();
	cookieHeader += _expiresToStr();
	cookieHeader += _httpOnlyToStr();
	cookieHeader += _secureToStr();
	cookieHeader += _sameSiteToStr();
	cookieHeader += "\r\n";

	return (cookieHeader);

}
std::string		Cookie::_keyValueToStr(void) const
{
	return (this->_keyValue.first + "=" + this->_keyValue.second);
}

std::string		Cookie::_domainToStr(void) const
{
	return (this->_domain != "" ? "; Domain=" + this->_domain : "");
}

std::string		Cookie::_pathToStr(void) const
{
	return (this->_path != "" ? "; Path=" + this->_path : "");
}

std::string		Cookie::_maxAgeToStr(void) const
{
	return (this->_maxAge != "" ? "; Max-Age=" + this->_maxAge : "");
}

std::string		Cookie::_expiresToStr(void) const
{
	return (this->_expires != "" ? "; Expires=" + this->_expires : "");
}

std::string		Cookie::_httpOnlyToStr(void) const
{
	return (this->_HttpOnly  ? "; HttpOnly" : "");
}

std::string		Cookie::_secureToStr(void) const
{
	return (this->_secure  ? "; Secure" : "");
}

std::string		Cookie::_sameSiteToStr(void) const
{
	switch (this->_sameSite)
	{
		case (LAX):
			return ("; SameSite=Lax");
		case (STRICT):
			return ("; SameSite=Strict");
		case (NONE):
			return ("; SameSite=None");
		default:
			return ("");
	}
}

