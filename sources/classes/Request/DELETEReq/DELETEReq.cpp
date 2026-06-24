/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETEReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/11 16:41:58 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETEReq.hpp"
#include <cstdio>

// Constructors/Destructor =====================================================
// DELETEReq::DELETEReq(const std::string &type, const std::string &header, const std::string &body):
// 	ARequest(type, header, body) {}

DELETEReq::DELETEReq(const ARequest &cpy):
	ARequest(cpy) {}

DELETEReq::DELETEReq(const DELETEReq &cpy):
	ARequest(cpy) {}

DELETEReq::~DELETEReq(void) {}

// Member functions ============================================================
uint16_t	DELETEReq::_removeResource(std::pair<int, std::string> &resource) const
{
	uint16_t status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	if (std::remove(resource.second.c_str()))
	{
		switch (errno)
		{
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

Response	DELETEReq::execute(void)
{
	const VirtualHost::UriInfo		&uriInfo = this->_vhost.getUriInfo(this->_uri);
	Response						resp(this->_fd, uriInfo.isCgiAllowed());

	resp.setCGI(uriInfo.isCgiAllowed());
	resp.setResourcePath(uriInfo.getRealPath());
	
	if (!uriInfo.isRequestAllowed(this->_method))
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_removeResource(resp.getResource()));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	return (resp);
}
