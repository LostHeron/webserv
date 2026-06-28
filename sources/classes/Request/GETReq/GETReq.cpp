/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 19:35:53 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTMLPageBuilder.hpp"
#include "GETReq.hpp"
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

// Constructors/Destructor =====================================================
GETReq::GETReq(const ARequest &src):
	ARequest(src) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
void	GETReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	uint16_t						&status = resp.getStatus();
	std::pair<int, std::string>		&resource = resp.getResource();
	std::string						&content = resp.getContent();

	DIR	*directory = this->_tryOpenDirectory(resource.second.c_str());
	if (directory && uriInfo.getIndex() != "")
	{
		this->_uri += uriInfo.getIndex() + "/";
		resource.second += "/" + uriInfo.getIndex();
		if (!resp.isCGI())
			resp.setCGI(uriInfo.isCgiExtAllowed(this->_getFileExtension(this->_uri)));

		closedir(directory);
		directory = this->_tryOpenDirectory(resource.second.c_str());
	}
	
	if (directory)
	{
		if (!uriInfo.isDirListAllowed())
		{
			closedir(directory);
			status = HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN;
			return;
		}
		content = HTMLPageBuilder::dirListingPage(directory, this->_uri);
		return;
	}
	else
	{
		switch (errno)
		{
			case (ENOTDIR):
				if (resp.isCGI())
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
				if (!resp.isCGI())
					status = HTTPStatus::S_ERR + HTTPStatus::INTERNAL;
				break;
		}
	}
}
