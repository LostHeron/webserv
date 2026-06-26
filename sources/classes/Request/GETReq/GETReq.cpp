/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/24 13:25:41 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTMLPageBuilder.hpp"
#include "GETReq.hpp"
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

// Constructors/Destructor =====================================================
GETReq::GETReq(const ARequest &src):
	ARequest(src) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
DIR	*GETReq::_tryOpenDirectory(const char *path) const
{
	DIR	*dir = opendir(path);

	return (dir);
}

int	GETReq::_tryOpenFile(const char *path) const
{
	int	fd = open(path, O_RDONLY);

	return (fd);
}

const std::string		getFileExtension(const std::string &uri)
{
	const size_t	pos = uri.find_last_of('.');

	return (pos == std::string::npos ? "" : uri.substr(pos + 1));
}

uint16_t		GETReq::_fetchResource(	std::pair<int, std::string> &resource,
										std::string &content, 
										const VirtualHost::UriInfo &uriInfo,
										bool &cgi)
{
	uint16_t	status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	DIR	*directory = this->_tryOpenDirectory(resource.second.c_str());
	if (directory && uriInfo.getIndex() != "")
	{
		this->_uri += uriInfo.getIndex() + "/";
		resource.second += "/" + uriInfo.getIndex();
		if (!cgi)
			cgi = uriInfo.isCgiExtAllowed(getFileExtension(this->_uri));

		closedir(directory);
		directory = this->_tryOpenDirectory(resource.second.c_str());
	}
	
	if (directory)
	{
		if (!uriInfo.isDirListAllowed())
		{
			closedir(directory);
			return (HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
		}
		content = HTMLPageBuilder::dirListingPage(directory, this->_uri);
		return (status);
	}
	else
	{
		switch (errno)
		{
			case (ENOTDIR):
				if (cgi)
					break;
				if ((resource.first = this->_tryOpenFile(resource.second.c_str())) >= 0)
					break;
				__attribute__((fallthrough));
			case (EACCES):
				status = HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN;
				break;
			case (ENOENT):
				status = HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND;
				break;
			default:
				if (!cgi)
					status = HTTPStatus::S_ERR + HTTPStatus::INTERNAL;
				break;
		}
	}
	return (status);
}

Response	GETReq::execute(void)
{
	const VirtualHost::UriInfo		&uriInfo = this->_vhost.getUriInfo(this->_uri);
	Response						resp(this->_fd, uriInfo.isCgiAllowed());

	resp.setResourcePath(uriInfo.getRealPath());
	resp.setCookies(this->_headerToCookies());


# ifdef	DEBUG
	for (it = resp.getCookies().begin(); it != resp.getCookies().end(); ++it)
		std::cout << *it << std::endl;
#endif


	if (!uriInfo.isRequestAllowed(this->_method))
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_fetchResource(resp.getResource(), resp.getContent(), uriInfo, resp.isCGI()));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	return (resp);
}
