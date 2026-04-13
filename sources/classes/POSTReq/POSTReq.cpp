/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTReq.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:39:47 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "POSTReq.hpp"

// Constructors/Destructor =====================================================
POSTReq::POSTReq(const uint8_t &type, const std::string &header, const std::string &body):
	ARequest(type, header, body) {}

POSTReq::POSTReq(const POSTReq &cpy):
	ARequest(cpy) {}

POSTReq::~POSTReq(void) {}

// Member functions ============================================================
void	POSTReq::execute(void)
{
	std::cout << "I AM A POST REQUEST!" << std::endl;
}
