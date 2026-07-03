/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Session.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 17:33:18 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 18:03:56 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__SESSION_HPP__
# define	__SESSION_HPP__

# include "Cookie.hpp"

# define SESSION_ID_CHAR	16

/*
class	Session
{
	public:
		Session(void);
		Session(std::map<std::string, Cookie> &cookies);
		~Session(void);

		std::string						getSessionId(void);
		std::map<std::string, Cookie>	&getSessionCookies(void);

		void							updateCookie(Cookie &cookie);
		void							deleteOldCookies(void);
		void							addCookie(Cookie &cookie);
		void							removeCookie(const std::string &key);

	private:
		std::map<std::string, Cookie>	_cookies;
};

		  */
#endif
