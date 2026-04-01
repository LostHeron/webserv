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

// Getters =====================================================================
const std::string	&AMessage::getMessage(void) const	{ return(this->_message); }
const std::string	&AMessage::getSender(void) const	{ return(this->_sender); }
const std::string	&AMessage::getTarget(void) const	{ return (this->_target); }
