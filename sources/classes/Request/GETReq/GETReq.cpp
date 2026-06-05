/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/31 17:14:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTMLPageBuilder.hpp"
#include "GETReq.hpp"
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

// Constructors/Destructor =====================================================
// GETReq::GETReq(void):
// 	ARequest(type, header, body) {}

GETReq::GETReq(const ARequest &cpy):
	ARequest(cpy) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
DIR	*GETReq::_tryOpenDirectory(const char *path) const
{
	DIR	*dir = opendir(path);

	if (dir)
		return (dir);
	return (NULL);
}

int	GETReq::_tryOpenFile(const char *path) const
{
	int	fd = open(path, O_RDONLY);

	return (fd);
}

Response	GETReq::execute(void)
{
	Response	resp(this->_fd);
	int			resourceFd = -1;
	uint16_t	status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	std::pair<std::string, bool> configSetting = _vhost.getPathReq(this->_uri, this->_method);
	const std::string resourcePath = configSetting.first;
	// TEMP DEBUG
	std::cout << "URI to fetch: " << this->_uri 
		<< " for real resourcePath: " << resourcePath
		<< " for method: " << this->_method 
		<< (configSetting.second ? " <ALLOWED>" : " <FORBIDEN>")
		<< std::endl;
	// TEMP DEBUG
	
	DIR	*directory = this->_tryOpenDirectory(resourcePath.c_str());
	if (directory)	
		resp.setContent(HTMLPageBuilder::dirListingPage(directory, this->_uri));
	else
	{
		switch (errno)
		{
			case (ENOTDIR):
				if ((resourceFd = this->_tryOpenFile(resourcePath.c_str())) >= 0)
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
		if (status >= HTTPStatus::C_ERR)
			resp.setContent(HTMLPageBuilder::errorPage(status));
	}

	// metadata settings
	resp.setResource(resourceFd, resourcePath);
	resp.setStatus(status);

	return (resp);
}
