/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:00:53 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/01 19:23:23 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__HTTPSTATUS_HPP__
# define	__HTTPSTATUS_HPP__

#include <iostream>
#include <stdint.h>

class	HTTPStatus
{
	public:

		static inline const std::string	getStatusMessage(const uint16_t statusCode);

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
			VARIANT_ALSO_NEGOTIATES,
			INSUFFICIENT_STORAGE,
			LOOP_DETECTED,
			NOT_EXTENDED = 10,
			NETWORK_AUTH_REQUIRED
		};

	private:
		static const char	*_getInfoMessage(const uint16_t &statusCode);
		static const char	*_getSuccessMessage(const uint16_t &statusCode);
		static const char	*_getRedirMessage(const uint16_t &statusCode);
		static const char	*_getClientErrorMessage(const uint16_t &statusCode);
		static const char	*_getServerErrorMessage(const uint16_t &statusCode);

};

#endif
