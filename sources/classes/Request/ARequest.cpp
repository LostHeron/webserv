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
#include <sys/stat.h>

// Construction/Destruction ====================================================
ARequest::ARequest(const InputSocket &IOMessage, const VirtualHost& vhost):
	AMessage(IOMessage.getFd()),
	_vhost(vhost),
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

const std::string						ARequest::_getFileExtension(const std::string &path)
{
	const size_t	pos = path.find_last_of('.');

	return (pos == std::string::npos ? "" : path.substr(pos));
}

void									ARequest::_splitCGIPathInfo(Response &resp)
{
	std::string	&pathInfo = resp.getPathInfo();
	std::string	&respResourcePath = resp.getResource().second;
	std::string	resourcePath = resp.getResource().second;
	size_t	pos = respResourcePath.find("/", respResourcePath.find_first_of("/") + 1);

	while (pos != std::string::npos)
	{
		resourcePath = respResourcePath.substr(0, pos);

		struct stat st;
		if (stat(resourcePath.c_str(), &st) == 0)
		{
			if (st.st_mode & S_IFREG)
			{
				pathInfo = respResourcePath.substr(pos + 1);
				respResourcePath = resourcePath;
				return;
			}
		}
		else
		{
			switch (errno)
			{
				case (EACCES):
					resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
					resp.setCGI(false);
					return;
				case (ENOENT):
					resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND);
					resp.setCGI(false);
					return;
			}
		}
		pos = respResourcePath.find("/", pos + 1);
	}
}

Response										ARequest::buildResponse(void)
{
	const VirtualHost::UriInfo		&uriInfo = this->_vhost.getUriInfo(this->_uri);
	Response						resp(this->_fd, false);

	resp.setResourcePath(uriInfo.getRealPath());
	resp.setRedir(uriInfo.isRedir());
	if (resp.isRedir())
	{
		resp.setStatus(HTTPStatus::REDIR + HTTPStatus::MOVED_PERM);
		return (resp);
	}

	resp.setCGI(uriInfo.isCgiAllowed());

	this->_splitCGIPathInfo(resp);
	if (!resp.isRedir() && !resp.isCGI())
	{
		resp.setCGI(uriInfo.isCgiExtAllowed(this->_getFileExtension(resp.getResource().second)));
		if (!resp.isCGI())
			resp.setResourcePath(uriInfo.getRealPath());
	}
	struct stat st;
	if (stat(resp.getResource().second.c_str(), &st) != 0)
		resp.setCGI(false);


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
