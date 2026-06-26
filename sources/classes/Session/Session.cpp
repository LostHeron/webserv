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

// void							Session::addCookie(Cookie &cookie);
// void							Session::updateCookie(Cookie &cookie);
// void							Session::removeCookie(std::string &key);
