/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 17:38:03 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/22 17:56:48 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookie.hpp"

// Constructors/Destructors ====================================================
Cookie::Cookie(void):
	_keyValue(),
	_domain(),
	_maxAge(),
	_expires(),
	_HttpOnly(false),
	_secure(false),
	_sameSite(NONE) {}

Cookie::~Cookie(void) {}

// Getters =====================================================================
kvPair			Cookie::getKeyValue(void)	{	return (this->_keyValue);	}
std::string		Cookie::getDomain(void)		{	return (this->_domain);		}
std::string		Cookie::getMaxAge(void)		{	return (this->_maxAge);		}
std::string		Cookie::getExpires(void) 	{	return (this->_expires);	}
bool			Cookie::gethttpOnly(void) 	{	return (this->_HttpOnly);	}
bool			Cookie::getSecure(void) 	{	return (this->_secure);		}
char			Cookie::getSameSite(void) 	{	return (this->_sameSite);	}


// Setters =====================================================================
void			Cookie::setKeyValue(const kvPair &kv)
{
	this->_keyValue = kv;
}

void			Cookie::setDomain(const std::string &domain)
{
	this->_domain = domain;
}

void			Cookie::setMaxAge(const std::string &age)
{
	this->_maxAge = age;
}

void			Cookie::setExpires(const std::string &date)
{
	this->_expires = date;
}

void			Cookie::sethttpOnly(const bool isHttpOnly)
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
