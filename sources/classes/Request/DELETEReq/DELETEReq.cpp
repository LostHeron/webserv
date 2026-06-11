/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETEReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:38:59 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETEReq.hpp"
#include <cstdio>

// Constructors/Destructor =====================================================
DELETEReq::DELETEReq(const ARequest &cpy):
	ARequest(cpy) {}

DELETEReq::DELETEReq(const DELETEReq &cpy):
	ARequest(cpy) {}

DELETEReq::~DELETEReq(void) {}

// Member functions ============================================================
uint16_t	DELETEReq::_removeResource(std::pair<int, std::string> &resource) const
{
	uint16_t status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	std::cout << "\n\n\n ===> RESOURCE:::::: " << resource.second << "\n\n\n\n" << std::endl;
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
	Response	resp(this->_fd);

	std::pair<std::string, bool> configSetting = this->_vhost.getPathReq(this->_uri, this->_method);
	resp.setResourcePath(configSetting.first);

	if (!configSetting.second) // always unauthorized
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_removeResource(resp.getResource()));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	return (resp);
}
