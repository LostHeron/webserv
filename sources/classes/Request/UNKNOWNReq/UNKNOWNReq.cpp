/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UNKNOWNReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:38:59 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UNKNOWNReq.hpp"

// Constructors/Destructor =====================================================
// UNKNOWNReq::UNKNOWNReq(const std::string &type, const std::string &header, const std::string &body):
// 	ARequest(type, header, body) {}

UNKNOWNReq::UNKNOWNReq(const ARequest &cpy):
	ARequest(cpy) {}

UNKNOWNReq::UNKNOWNReq(const UNKNOWNReq &cpy):
	ARequest(cpy) {}

UNKNOWNReq::~UNKNOWNReq(void) {}

// Member functions ============================================================
void	UNKNOWNReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	(void) uriInfo;

	resp.setCGI(true);
	resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_ALLOWED_METHOD);
}
