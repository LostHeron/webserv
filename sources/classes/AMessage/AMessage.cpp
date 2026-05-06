/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:06:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:19:44 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"
// Constructors/Destructors ====================================================
// AMessage::AMessage(void):
// 	_fd(-1) {}

AMessage::AMessage(const int &fd):
	_fd(fd) {}

AMessage::AMessage(const AMessage &cpy):
	_fd(cpy._fd) {}

AMessage::~AMessage(void) {}

// Setters =====================================================================
// void	AMessage::setFd(const int &fd) 		{ this->_fd = fd; }

// Getters =====================================================================
const int	&AMessage::getFd(void) const	{ return(this->_fd); }
