/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/22 17:25:52 by abetemps         ###   ########.fr       */
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

uint16_t		GETReq::_fetchResource(std::pair<int, std::string> &resource, std::string &content, const VirtualHost::UriInfo &uriInfo)
{
	uint16_t	status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	DIR	*directory = this->_tryOpenDirectory(resource.second.c_str());
	if (directory)
	{
		if (uriInfo.getIndex() != "")
		{
			this->_uri += uriInfo.getIndex() + "/";
			resource.second += "/" + uriInfo.getIndex();

			#	ifdef	DEBUG
			std::cout << "URI: '" << this->_uri << "'\nResp.second (concat index): " << resource.second << std::endl;
			#	endif

			directory = this->_tryOpenDirectory(resource.second.c_str());
		}
	}
	
	if (directory)
	{
		if (!uriInfo.isDirListAllowed())
			return (HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
		content = HTMLPageBuilder::dirListingPage(directory, this->_uri);
		return (status);
	}
	else
	{
		switch (errno)
		{
			case (ENOTDIR):
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
	

	resp.setCGI(uriInfo.isCgiAllowed());
	resp.setResourcePath(uriInfo.getRealPath());


	if (!uriInfo.isRequestAllowed(this->_method))
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_fetchResource(resp.getResource(), resp.getContent(), uriInfo));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	// test cookies
	std::vector<Response::cookie> cookies;
	std::pair<std::string, std::string> cookie;

	cookie.first = "id";
	cookie.second = "yolo";
	cookies.push_back(cookie);

	cookie.first = "eat";
	cookie.second = "soup";
	cookies.push_back(cookie);

	cookie.first = "coco";
	cookie.second = "jweber";
	cookies.push_back(cookie);

	resp.setCookies(cookies);
	// test cookies

	return (resp);
}
