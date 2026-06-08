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

	if (dir)
		return (dir);
	return (NULL);
}

int	GETReq::_tryOpenFile(const char *path) const
{
	int	fd = open(path, O_RDONLY);

	return (fd);
}

uint16_t		GETReq::_fetchResource(std::pair<int, std::string> &resource, std::string &content) const
{
	uint16_t	status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	DIR	*directory = this->_tryOpenDirectory(resource.second.c_str());
	if (directory)	
		content = HTMLPageBuilder::dirListingPage(directory, this->_uri);
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
	Response	resp(this->_fd);

	std::pair<std::string, bool> configSetting = this->_vhost.getPathReq(this->_uri, this->_method);
	resp.setResourcePath(configSetting.first);

	if (!configSetting.second)
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_fetchResource(resp.getResource(), resp.getContent()));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	return (resp);
}

	// // TEMP DEBUG
	// std::cout << "URI to fetch: " << this->_uri 
	// 	<< " for real resourcePath: " << resourcePath
	// 	<< " for method: " << this->_method 
	// 	<< (configSetting.second ? " <ALLOWED>" : " <FORBIDEN>")
	// 	<< std::endl;
	// // TEMP DEBUG
