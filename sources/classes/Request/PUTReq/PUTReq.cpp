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
#include <fstream>

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
	// if (this->_isWriteMode()) { throw FORBIDDEN; }
	std::ofstream	of;
	of.open(upPath.c_str());
	if (!of.is_open())
	{
		switch (errno)
		{
			case(EACCES):
				resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
				return;	
			case(ENOENT):
				resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND);
				return;	
			default:
				resp.setStatus(HTTPStatus::S_ERR + HTTPStatus::INTERNAL);
				return;
		}
	}
	// of << this->_body;
	of << "This body" ;
}

void	PUTReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	# define TMP_PUT_PATH "/home/abetemps/temp_webserv_root/put/"
	(void) uriInfo;

	std::string	upPath = TMP_PUT_PATH + this->_uri;//uriInfo.getRealPath(this->_uri);
	this->_uploadFile(resp, upPath);
}
