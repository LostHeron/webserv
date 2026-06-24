/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 17:31:41 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/24 11:35:18 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__COOKIE_HPP__
# define	__COOKIE_HPP__

# include <iostream>
# include <cstdlib>

# define	SESSION_COOKIE_KEY	"id"

class		Cookie
{
	public:
		typedef	std::pair<std::string, std::string>	kvPair;

		Cookie(void);
		~Cookie(void);

		enum	e_sameSite
		{
			LAX,
			STRICT,
			NONE
		};

		kvPair			getKeyValue(void);
		std::string		getDomain(void);
		std::string		getPath(void);
		std::string		getMaxAge(void);
		std::string		getExpires(void);
		bool			gethttpOnly(void);
		bool			getSecure(void);
		char			getSameSite(void);

		void			setKeyValue(const kvPair &kv);
		void			setDomain(const std::string &domain);
		void			setPath(const std::string &path);
		void			setMaxAge(const std::string &age);
		void			setExpires(const std::string &date);
		void			setHttpOnly(const bool isHttpOnly);
		void			setSecure(const bool isSecure);
		void			setSameSite(const char sameSiteFlag);

		std::string		cookieToStr(void) const;

	private:
		kvPair			_keyValue;
		std::string		_domain;
		std::string		_path;
		std::string		_maxAge;
		std::string		_expires;
		bool			_HttpOnly;
		bool			_secure;
		char			_sameSite;
		
		std::string		_keyValueToStr(void) const;
		std::string		_domainToStr(void) const;
		std::string		_pathToStr(void) const;
		std::string		_maxAgeToStr(void) const;
		std::string		_expiresToStr(void) const;
		std::string		_httpOnlyToStr(void) const;
		std::string		_secureToStr(void) const;
		std::string		_sameSiteToStr(void) const;
};

std::ostream	&operator<<(std::ostream &os, const Cookie &cookie);

#endif
