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

# define	THEME_COOKIE_DEFAULT	"light"
# define	DEFAULT_LIFETIME_SEC	900		// 15 minutes

class		Cookie
{
	public:
		typedef	std::pair<std::string, std::string>	kvPair;

		Cookie(void);
		Cookie(kvPair keyValue);
		~Cookie(void);

		enum			e_sameSite
		{
			LAX,
			STRICT,
			NONE
		};

		enum			e_permCookies
		{
			SESSION,
			THEME,
			PERM_COOKIES_QTY
		};

		static const char	*permanentCookies[PERM_COOKIES_QTY];
		static bool			isPermanentCookie(const std::string &key);

		kvPair			getKeyValue(void);
		std::string		getDomain(void);
		std::string		getPath(void);
		long			getMaxAge(void);
		std::string		getExpires(void);
		bool			gethttpOnly(void);
		bool			getSecure(void);
		char			getSameSite(void);

		void			setKeyValue(const kvPair &kv);
		void			setDomain(const std::string &domain);
		void			setPath(const std::string &path);
		void			setMaxAge(const long &age);
		void			setExpires(const std::string &date);
		void			setHttpOnly(const bool isHttpOnly);
		void			setSecure(const bool isSecure);
		void			setSameSite(const char sameSiteFlag);

		std::string		cookieToStr(void) const;

	private:
		kvPair			_keyValue;
		std::string		_domain;
		std::string		_path;
		long			_maxAge;
		std::string		_expires;
		bool			_HttpOnly;
		bool			_secure;
		char			_sameSite;
		bool			_permanent; // if part of permanent cookies key
		
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
