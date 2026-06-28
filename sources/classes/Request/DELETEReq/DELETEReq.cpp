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
DELETEReq::DELETEReq(const ARequest &cpy):
	ARequest(cpy) {}

DELETEReq::DELETEReq(const DELETEReq &cpy):
	ARequest(cpy) {}

DELETEReq::~DELETEReq(void) {}

// Member functions ============================================================
void	DELETEReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	(void) uriInfo;
	uint16_t 					&status = resp.getStatus();
	std::pair<int, std::string>	&resource = resp.getResource();

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
}
