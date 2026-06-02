/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPStatus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:34:00 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/01 19:37:55 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"

inline const std::string HTTPStatus::getStatusMessage(const uint16_t statusCode)
{
	switch (statusCode / 100)
	{
		case (1):
			return (HTTPStatus::_getInfoMessage(statusCode - HTTPStatus::INFO));
		case (2):
			return (HTTPStatus::_getSuccessMessage(statusCode - HTTPStatus::SUCCESS));
		case (3):
			return (HTTPStatus::_getRedirMessage(statusCode - HTTPStatus::REDIR));
		case (4):
			return (HTTPStatus::_getClientErrorMessage(statusCode - HTTPStatus::C_ERR));
		case (5):
			return (HTTPStatus::_getServerErrorMessage(statusCode - HTTPStatus::S_ERR));
	};
	return (NULL);
}

inline const char	*HTTPStatus::_getInfoMessage(const uint16_t &statusCode)
{
	switch (statusCode)
	{
		case (HTTPStatus::CONTINUE):
			return ("Continue");
		case (HTTPStatus::SWITCH):
			return ("Switching Protocols");
		case (HTTPStatus::PROCESSING):
			return ("Processing");
		case (HTTPStatus::EARLY_HINTS):
			return ("Early Hints");
	};
	return (NULL);
}

inline const char	*HTTPStatus::_getSuccessMessage(const uint16_t &statusCode)
{
	switch (statusCode)
	{
		case (HTTPStatus::OK):
			return ("OK");
		case (HTTPStatus::CREATED):
			return ("Created");
		case (HTTPStatus::ACCEPTED):
			return ("Accepted");
		case (HTTPStatus::NON_AUTHORITATIVE_INFO):
			return ("Non-Authoritative Information");
		case (HTTPStatus::NO_CONTENT):
			return ("No Content");
		case (HTTPStatus::RESET_CONTENT):
			return ("Reset Content");
		case (HTTPStatus::PARTIAL_CONTENT):
			return ("Partial Content");
		case (HTTPStatus::MULTI_STATUS):
			return ("Multi-Status");
		case (HTTPStatus::ALREADY_REPORTED):
			return ("Already Reported");
		case (HTTPStatus::IM_USED):
			return ("Im Used");
	};
	return (NULL);
}

inline const char	*HTTPStatus::_getRedirMessage(const uint16_t &statusCode)
{
	switch (statusCode)
	{
		case (HTTPStatus::MULTIPLE_CHOICES):
			return ("Multiple Choices");
		case (HTTPStatus::MOVED_PERM):
			return ("Moved Permanently");
		case (HTTPStatus::FOUND):
			return ("Found");
		case (HTTPStatus::SEE_OTHER):
			return ("See Other");
		case (HTTPStatus::NOT_MODIFIED):
			return ("Not Modified");
		case (HTTPStatus::USE_PROXY):
			return ("Use Proxy");
		case (HTTPStatus::UNUSED):
			return ("Unused");
		case (HTTPStatus::TEMP_REDIR):
			return ("Temporary Redirect");
		case (HTTPStatus::PERM_REDIR):
			return ("Permanent Redirect");
	};
	return (NULL);
}

inline const char	*HTTPStatus::_getClientErrorMessage(const uint16_t &statusCode)
{
	switch (statusCode)
	{
		case (HTTPStatus::BAD_REQ):
			return ("Bad Request");
		case (HTTPStatus::UNAUTHORIZED):
			return ("Unauthorized");
		case (HTTPStatus::PAYMENT_REQUIRED):
			return ("Payment Required");
		case (HTTPStatus::FORBIDDEN):
			return ("Forbidden");
		case (HTTPStatus::NOT_FOUND):
			return ("Not Found");
		case (HTTPStatus::NOT_ALLOWED_METHOD):
			return ("Method Not Allowed");
		case (HTTPStatus::PROXY_AUTH_REQUIRED):
			return ("Proxy Authentication Required");
		case (HTTPStatus::TIMEOUT):
			return ("Request Timeout");
		case (HTTPStatus::CONFLICT):
			return ("Conflict");
		case (HTTPStatus::GONE):
			return ("Gone");
		case (HTTPStatus::LENGTH_REQUIRED):
			return ("Length Required");
		case (HTTPStatus::PRECONDITION_FAILED):
			return ("Precondition Failed");
		case (HTTPStatus::TOO_LARGE):
			return ("Payload Too Large");
		case (HTTPStatus::TOO_LONG_URI):
			return ("URI Too Long");
		case (HTTPStatus::UNSUPPORTED_MEDIA_TYPE):
			return ("Unsupported Media Type");
		case (HTTPStatus::RANGE_NOT_SATISFIABLE):
			return ("Range Not Satisfiable");
		case (HTTPStatus::EXPECTATION_FAILED):
			return ("Expectation Failed");
		case (HTTPStatus::TEAPOT):
			return ("I'm a Teapot");
		case (HTTPStatus::MISDIRECTED_REQ):
			return ("Misdirected Request");
		case (HTTPStatus::UNPROCESSABLE_CONTENT):
			return ("Unprocessable Content");
		case (HTTPStatus::LOCKED):
			return ("Locked");
		case (HTTPStatus::FAILED_DEP):
			return ("Failed Dependency");
		case (HTTPStatus::TOO_EARLY):
			return ("Too Early");
		case (HTTPStatus::UPGRADE_REQUIRED):
			return ("Upgrade Required");
		case (HTTPStatus::PRECONDITION_REQUIRED):
			return ("Precondition Required");
		case (HTTPStatus::TOO_MANY_REQUEST):
			return ("Too Many Requests");
		case (HTTPStatus::TOO_LARGE_HDR_FIELDS):
			return ("Request Header Fields Too Large");
		case (HTTPStatus::LEGAL):
			return ("Unavailable For Legal Reasons");
	};
	return (NULL);
}

inline const char	*HTTPStatus::_getServerErrorMessage(const uint16_t &statusCode)
{
	switch (statusCode)
	{
		case (HTTPStatus::INTERNAL):
			return ("Internal Server Error");
		case (HTTPStatus::NOT_IMPLEMENTED):
			return ("Not Implemented");
		case (HTTPStatus::BAD_GATEWAY):
			return ("Bad Gateway");
		case (HTTPStatus::TIMEOUT_GATEWAY):
			return ("Gateway Timeout");
		case (HTTPStatus::HTTP_VERSION_NOT_SUPPORTED):
			return ("HTTP Version Not Supported");
		case (HTTPStatus::VARIANT_ALSO_NEGOTIATES):
			return ("Variant Also Negotiates");
		case (HTTPStatus::INSUFFICIENT_STORAGE):
			return ("Insufficient Storage");
		case (HTTPStatus::LOOP_DETECTED):
			return ("Loop Detected");
		case (HTTPStatus::NOT_EXTENDED):
			return ("Not Extended");
		case (HTTPStatus::NETWORK_AUTH_REQUIRED):
			return ("Network Authentication Required");
	};
	return (NULL);
}
