/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:39:13 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETReq.hpp"

// Constructors/Destructor =====================================================
GETReq::GETReq(const uint8_t &type, const std::string &header, const std::string &body):
	ARequest(type, header, body) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
void	GETReq::execute(void)
{
	std::cout << "I AM A GET REQUEST!" << std::endl;
}
