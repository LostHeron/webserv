/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/28 11:47:03 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef		__AREQUEST_HPP__
# define	__AREQUEST_HPP__

# include "AMessage.hpp"
# include "Response.hpp"
# include "InputSocket.hpp"
#include "VirtualHost.hpp"
# include <vector>
# include <unistd.h>
# include <fcntl.h>


# define TEMP_ROOT "/home/goinfre/tmp"


class	ARequest: public AMessage
{
	public:
		ARequest(const InputSocket &IOMessage, const VirtualHost& vhost);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest							&operator=(const ARequest &assign);
                                			
		virtual Response					execute(void) = 0; // instant execute immediate actions, then build Response according to execution metadata, status...

		const std::string										&getMethod(void) const;
		const std::string										&getUri(void) const;
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

		enum				e_reqStatus
		{
			INFO = 100,
			SUCCESS = 200,
			REDIR = 300,
			C_ERR = 400,
			S_ERR = 500
		};

		enum				e_reqStatusInfo
		{
			CONTINUE,
			SWITCH,
			PROCESSING,
			EARLY_HINTS
		};

		enum				e_reqStatusSuccess
		{
			OK,
			CREATED,
			ACCEPTED,
			NON_AUTHORITATIVE_INFO,
			NO_CONTENT,
			RESET_CONTENT,
			PARTIAL_CONTENT,
			MULTI_STATUS,
			ALREADY_REPORTED,
			IM_USED = 26
		};

		enum				e_reqStatusRedir
		{
			MULTIPLE_CHOICES,
			MOVED_PERM,
			FOUND,
			SEE_OTHER,
			NOT_MODIFIED,
			USE_PROXY,
			UNUSED,
			TEMP_REDIR,
			PERM_REDIR
		};

		enum				e_reqStatusClientError
		{
			BAD_REQ,
			UNAUTHORIZED,
			PAYMENT_REQUIRED,
			FORBIDDEN,
			NOT_FOUND,
			NOT_ALLOWED_METHOD,
			PROXY_AUTH_REQUIRED,
			TIMEOUT,
			CONFLICT,
			GONE,
			LENGTH_REQUIRED,
			PRECONDITION_FAILED,
			TOO_LARGE,
			TOO_LONG_URI,
			UNSUPPORTED_MEDIA_TYPE,
			RANGE_NOT_SATISFIABLE,
			EXPECTATION_FAILED,
			TEAPOT,
			MISDIRECTED_REQ = 21,
			UNPROCESSABLE_CONTENT,
			LOCKED,
			FAILED_DEP,
			TOO_EARLY,
			UPGRADE_REQUIRED,
			PRECONDITION_REQUIRED = 28,
			TOO_MANY_REQUEST,
			TOO_LARGE_HDR_FIELDS = 31,
			LEGAL = 51
		};

		enum				e_reqStatusServerError
		{
			INTERNAL,
			NOT_IMPLEMENTED,
			BAD_GATEWAY,
			TIMEOUT_GATEWAY,
			HTTP_VERSION_NOT_SUPPORTED,
			VARIANT_ALSO_NEGOTIATE,
			INSUFFICENT_STORAGE,
			LOOP_DETECTED,
			NOT_EXTENDED = 10,
			NETWORK_AUTH_REQUIRED
		};

	protected:
		const VirtualHost					&_vhost;
		const std::string					_method;
		const std::string				  	_uri;
		const std::string				  	_version;
		const string_map					_header;
};

#endif
