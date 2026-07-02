/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PUTReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:38:59 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PUTReq.hpp"

// Constructors/Destructor =====================================================
// PUTReq::PUTReq(const std::string &type, const std::string &header, const std::string &body):
// 	ARequest(type, header, body) {}

PUTReq::PUTReq(const ARequest &cpy):
	ARequest(cpy) {}

PUTReq::PUTReq(const PUTReq &cpy):
	ARequest(cpy) {}

PUTReq::~PUTReq(void) {}

// Member functions ============================================================
void	PUTReq::_uploadFile(Response &resp, const std::string &upPath)
{
	(void)	 resp;
	(void)	 upPath;
}

void	PUTReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	(void) uriInfo;

	// if (std::string upPath = uriInfo.getUploadPath())
	// this->_uploadFile(upPath);
	resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_ALLOWED_METHOD);
}
