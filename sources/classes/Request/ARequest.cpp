/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 18:36:52 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 18:38:03 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ARequest.hpp"
#include "VirtualHost.hpp"
#include <algorithm>

// Construction/Destruction ====================================================
ARequest::ARequest(const InputSocket &IOMessage, const VirtualHost& vhost):
	AMessage(IOMessage.getFd()),
	_vhost(const_cast<VirtualHost &>(vhost)),
	_method(IOMessage.getMethod()),
	_uri(IOMessage.getUri()),
	_version(IOMessage.getVersion()),
	_header(IOMessage.getHeaders()) {}

ARequest::ARequest(const ARequest &cpy):
	AMessage(cpy._fd),
	_vhost(cpy._vhost),
	_method(cpy._method),
	_uri(cpy._uri),
	_version(cpy._version),
	_header(cpy._header) {}

ARequest::~ARequest(void) {}


// Ops overloading =============================================================
ARequest			&ARequest::operator=(const ARequest &assign){ (void) assign; return (*this); }

// Setters =====================================================================


// Getters =====================================================================
const std::string										&ARequest::getMethod(void)	const	{ return(this->_method); }
std::string												ARequest::getUri(void) 				{ return(this->_uri); }
const std::string										&ARequest::getVersion(void) const 	{ return(this->_version); }
const std::map<std::string, std::vector<std::string> >	&ARequest::getHeader(void)	const 	{ return(this->_header); }

// Member Functions ============================================================
DIR						*ARequest::_tryOpenDirectory(const char *path) const
{
	DIR	*dir = opendir(path);

	return (dir);
}

int						ARequest::_tryOpenFile(const char *path) const
{
	int	fd = open(path, O_RDONLY);

	return (fd);
}

const std::string		ARequest::_getFileExtension(const std::string &path)
{
	const size_t	pos = path.find_last_of('.');

	return (pos == std::string::npos ? "" : path.substr(pos));
}


Response										ARequest::buildResponse(void)
{
	const VirtualHost::UriInfo		&uriInfo = this->_vhost.getUriInfo(this->_uri);
	Response						resp(this->_fd, uriInfo.isCgiAllowed());

	resp.setRedir(uriInfo.isRedir());
	resp.setResourcePath(uriInfo.getRealPath());
	if (!resp.isCGI())
		resp.setCGI(uriInfo.isCgiExtAllowed(this->_getFileExtension(this->_uri)));

	std::map<std::string, Cookie> receivedCookies = this->_headerToCookies();
	resp.setCookies(this->_updateCookies(receivedCookies));

	if (resp.isRedir())
		resp.setStatus(HTTPStatus::REDIR + HTTPStatus::MOVED_PERM);

	if (!uriInfo.isRequestAllowed(this->_method))
	{
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_ALLOWED_METHOD);
		resp.setCGI(false);
	}
	else if (!resp.isCGI())
		this->_execute(resp, uriInfo);


	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	return (resp);

}

std::map<std::string, Cookie>					ARequest::_headerToCookies(void)
{
	std::map<std::string, Cookie>	cookies;

	if (this->_header.count("cookie") == 0)
		return (cookies);

	std::vector<std::string>::iterator	headersIt;

	for (headersIt = this->_header["cookie"].begin(); headersIt != this->_header["cookie"].end(); ++headersIt)
	{
		std::vector<std::string>	splitCookies;
		std::string					elem(*headersIt);

		size_t	posElem = elem.find_first_of("; ");
		if (posElem == std::string::npos)
			splitCookies.push_back(elem);
		while (posElem != std::string::npos)
		{
			posElem = elem.find_first_of("; ");
			splitCookies.push_back(elem.substr(0, posElem));
			elem = elem.substr(posElem + 2);
		}

		std::vector<std::string>::iterator	elemIt;
		for (elemIt = splitCookies.begin(); elemIt != splitCookies.end(); ++elemIt)
		{
			size_t		posKV = elemIt->find('=');

			std::string	key = elemIt->substr(0, posKV);
			std::string	value = elemIt->substr(posKV + 1);

			Cookie		cookie(Cookie::kvPair(key, value));
			cookies[key] = cookie;
		}
	}
	return (cookies);
}

std::map<std::string, Cookie>					&ARequest::_updateCookies(std::map<std::string, Cookie> &cookies)
{
	std::map<std::string, Session>			&sessions = this->_vhost.getSessions();

	this->_vhost.removeOldSessions();

	if (cookies.count(Cookie::permanentCookies[Cookie::SESSION]) != 1)
	{
		std::string	id;
		do
			id = this->_vhost.buildSessionId();
		while (!this->_vhost.isIdAvailable(id));

		cookies[Cookie::permanentCookies[Cookie::SESSION]] = Cookie(Cookie::kvPair(Cookie::permanentCookies[Cookie::SESSION], id));
	}

	const std::string id = cookies[Cookie::permanentCookies[Cookie::SESSION]].getKeyValue().second;
	if (sessions.count(id) != 1)
	{
		Session currentSession(cookies);

		this->_vhost.addSession(currentSession);
	}
	else
	{
		this->_vhost.updateSession(id, cookies);
	}
	
	return (cookies);
}
