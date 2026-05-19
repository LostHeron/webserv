/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:19:10 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef		__AREQUEST_HPP__
# define	__AREQUEST_HPP__

# include "AMessage.hpp"
# include "Response.hpp"
# include "IOFd.hpp"
# include <unistd.h>
# include <fcntl.h>


# define TEMP_ROOT "/home/abetemps/temp_webserv_root"


class	ARequest: public AMessage
{
	public:
		ARequest(const IOFd &IOMessage);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest							&operator=(const ARequest &assign);
                                			
		virtual Response					execute(void) = 0; // instant execute immediate actions, then build Response according to execution metadata, status...

		const std::string					&getMethod(void) const;
		const std::string					&getUri(void) const;
		const std::string					&getVersion(void) const;
		const std::vector< std::string >	&getHeader(void) const;
		const std::vector<unsigned char>	&getBody(void) const;

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
			C_ERROR = 400
			S_ERROR = 500
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
			IM_USED
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
			MISDIRECTED_REQ,
			UNPROCESSABLE_CONTENT,
			LOCKED,
			FAILED_DEP,
			TOO_EARLY,
			UPGRADE_REQUIRED,
			PRECONDITION_REQUIRED,
			TOO_MANY_REQUEST,
			TOO_LARGE_HDR_FIELDS,
			LEGAL
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
			NOT_EXTENDED,
			NETWORK_AUTH_REQUIRED
		};

	protected:
		const std::string				  	_method;
		const std::string				  	_uri;
		const std::string				  	_version;
		const std::vector<std::string>		_header;
		const std::vector<unsigned char>	_body;
};

#endif
