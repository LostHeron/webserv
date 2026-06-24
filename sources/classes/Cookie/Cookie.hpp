/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 17:31:41 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/22 17:37:53 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__COOKIE_HPP__
# define	__COOKIE_HPP__

# include <iostream>
# include <cstdlib>

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
		std::string		getMaxAge(void);
		std::string		getExpires(void);
		bool			gethttpOnly(void);
		bool			getSecure(void);
		char			getSameSite(void);

		void			setKeyValue(const kvPair &kv);
		void			setDomain(const std::string &domain);
		void			setMaxAge(const std::string &age);
		void			setExpires(const std::string &date);
		void			sethttpOnly(const bool isHttpOnly);
		void			setSecure(const bool isSecure);
		void			setSameSite(const char sameSiteFlag);

	private:
		kvPair			_keyValue;
		std::string		_domain;
		std::string		_maxAge;
		std::string		_expires;
		bool			_HttpOnly;
		bool			_secure;
		char			_sameSite;
		
};

#endif
