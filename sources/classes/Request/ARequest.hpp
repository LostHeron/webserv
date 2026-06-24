/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/01 18:53:40 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef		__AREQUEST_HPP__
# define	__AREQUEST_HPP__

# include "HTTPStatus.hpp"
# include "AMessage.hpp"
# include "Response.hpp"
# include "InputSocket.hpp"
# include "VirtualHost.hpp"
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>


typedef HTTPStatus::e_reqStatus	e_reqStatus;
typedef HTTPStatus::e_reqStatusInfo e_reqStatusInfo;
typedef HTTPStatus::e_reqStatusSuccess e_reqStatusSuccess;
typedef HTTPStatus::e_reqStatusRedir e_reqStatusRedir;
typedef HTTPStatus::e_reqStatusClientError e_reqStatusClientError;
typedef HTTPStatus::e_reqStatusServerError e_reqStatusServerError;

class	ARequest: public AMessage
{
	public:
		ARequest(const InputSocket &IOMessage, const VirtualHost& vhost);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest												&operator=(const ARequest &assign);

		virtual Response										execute(void) = 0;

		const std::string										&getMethod(void) const;
		std::string												getUri(void);
		const std::string										&getVersion(void) const;
		const std::map< std::string, std::vector<std::string> >	&getHeader(void) const;

		enum				e_reqType
		{
			GET,
			POST,
			DELETE,
			UNKNOWN,
			TYPE_QTY,
		};

	protected:
		std::vector<Cookie>	_headerToCookie(void);
		const VirtualHost	&_vhost;
		const std::string	_method;
		std::string			_uri;
		const std::string	_version;
	 	string_map			_header;

};

#endif
