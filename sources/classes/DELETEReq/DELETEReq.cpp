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

// Constructors/Destructor =====================================================
DELETEReq::DELETEReq(const uint8_t &type, const std::string &header, const std::string &body):
	ARequest(type, header, body) {}

DELETEReq::DELETEReq(const DELETEReq &cpy):
	ARequest(cpy) {}

DELETEReq::~DELETEReq(void) {}

// Member functions ============================================================
void	DELETEReq::execute(void)
{
	std::cout << "I AM A DELETE REQUEST!" << std::endl;
}
