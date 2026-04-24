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
AMessage::AMessage(void):
	_message(""),
	_target(""),
	_sender("") {}

AMessage::AMessage(const std::string &message, const std::string &target, const std::string &sender):
	_message(message),
	_target(target),
	_sender(sender) {}

AMessage::AMessage(const AMessage &cpy):
	_message(cpy._message),
	_target(cpy._target),
	_sender(cpy._sender) {}

AMessage::~AMessage(void) {}

// Setters =====================================================================
void	AMessage::setMessage(const std::string &message)	{ this->_message = message; }
void	AMessage::setSender(const std::string &sender) 		{ this->_sender = sender; }
void	AMessage::setTarget(const std::string &target) 		{ this->_target = target; }

// Getters =====================================================================
const std::string	&AMessage::getMessage(void) const	{ return(this->_message); }
const std::string	&AMessage::getSender(void) const	{ return(this->_sender); }
const std::string	&AMessage::getTarget(void) const	{ return (this->_target); }
