/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:32:53 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 18:56:13 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Session.hpp"

// Construction/Destruction ====================================================
Session::Session(void) {}

Session::Session(std::map<std::string, Cookie> &cookies): _cookies(cookies) {}

Session::~Session(void) {}

// Member functions ============================================================
std::string						Session::getSessionId(void)
{
	if (this->_cookies.count(Cookie::permanentCookies[Cookie::SESSION]))
		return (this->_cookies[Cookie::permanentCookies[Cookie::SESSION]].getKeyValue().second);
	return ("");
}

std::map<std::string, Cookie>	&Session::getSessionCookies(void)
{
	return (this->_cookies);
}

void							Session::updateCookie(Cookie &cookie)
{
	if (this->_cookies.count(cookie.getKeyValue().first) != 1)
		this->addCookie(cookie);
	else
		this->_cookies[cookie.getKeyValue().first] = cookie;
}

void							Session::addCookie(Cookie &cookie)
{
	this->_cookies[cookie.getKeyValue().first] = cookie;
}

void							Session::removeCookie(const std::string &key)
{
	this->_cookies.erase(key);
}

void							Session::deleteOldCookies(void)
{
	const time_t							now = time(NULL);
	std::map<std::string, Cookie>::iterator	iter;

	for (iter = this->_cookies.begin(); iter != this->_cookies.end(); ++iter)
	{
		if (std::difftime(now, iter->second.getInitializationDate()) >= iter->second.getMaxAge())
			this->removeCookie(iter->first);
	}
}
