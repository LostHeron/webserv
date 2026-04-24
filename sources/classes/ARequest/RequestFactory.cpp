/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 18:47:27 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestFactory.hpp"

// Static init =================================================================
const AFactory<ARequest>::_constructor	RequestFactory::_constructorsArray[] =
{
	&RequestFactory::_newElement<GETReq>,
	&RequestFactory::_newElement<POSTReq>,
	&RequestFactory::_newElement<DELETEReq>
};

// Constructors/Destructor =====================================================
RequestFactory::RequestFactory(void):
	AFactory<ARequest>() {}

RequestFactory::RequestFactory(const RequestFactory &cpy):
	AFactory<ARequest>(cpy) {}

RequestFactory::~RequestFactory(void) {}


// Ops overloading =============================================================
RequestFactory	&RequestFactory::operator=(const RequestFactory &assign) { (void) assign; return (*this); }


// Getters ======================================================================
const AFactory<ARequest>::_constructor		*RequestFactory::_getConstructors(void) const
{
	return (RequestFactory::_constructorsArray);
}

// Member functions =============================================================
void			RequestFactory::execute(void)
{
	std::cout << "Cannot execute Factory class." << std::endl;
}

uint8_t			RequestFactory::_determineElement(void) const
{
	if (!this->_checkHeader())
		return (UNKNOWN);

	if (this->_type == "GET")
		return (ARequest::GET);
	else if (this->_type == "POST" && this->_checkBody())
		return (ARequest::POST);
	else if (this->_type == "DELETE")
		return (ARequest::DELETE);
	else
		return (UNKNOWN);
}


// Tests =======================================================================
uint8_t		RequestFactory::_checkHeader(void) const
{
	// check if parsed header "looks like" a valid header
	return (1); // OK
}

uint8_t		RequestFactory::_checkBody(void) const
{
	// if requested, check if parsed body "looks like" a valid body
	return (0); // KO
}
