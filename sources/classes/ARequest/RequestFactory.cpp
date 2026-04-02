/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:45:15 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestFactory.hpp"

// Static init =================================================================
const	AFactory<ARequest>::_constructorsArray[TYPES_QTY] =
{
	&AFactory::newElement<GETReq>,
	&AFactory::newElement<POSTReq>,
	&AFactory::newElement<DELETEReq>
};

const	AFactory<ARequest>::_testsArray[] =
{
	// &_checkType,
	&_checkHeader,
	&_checkBody
};


// Constructors/Destructor =====================================================
RequestFactory::RequestFactory(void):
	_type(UNKNOWN),
	_header(0),
	_body(0) {}

RequestFactory::RequestFactory(const RequestFactory &cpy):
	_type(cpy._type),
	_header(cpy._header),
	_body(cpy._body) {}

RequestFactory::~RequestFactory(void) {}


// Ops overloading =============================================================
RequestFactory	&RequestFactory::operator=(const RequestFactory &assign) { (void) assign; return (*this); }


// Member functions =============================================================
ARequest		*RequestFactory::_newElement();

ARequest		*RequestFactory::createElement(void) const
{
	return (AFactory<ARequest>::_constructorsArray[determineElement()]());
}

uint8_t			RequestFactory::determineElement(void) const
{
	uint8_t	type;

	type = UNKNOWN;
	for (int i = 0; i < 2; ++i)
	{
		if (type = AFactory<ARequest>::_testsArray() == UNKNOWN)
			break;
	}
	return (type);
}


// Tests =======================================================================
uint8_t		RequestFactory::_checkType(void) const
{
	if (this->_type == "GET")
		return (GET);
	else if (this->_type == "POST")
		return (POST);
	else if (this->_type == "DELETE")
		return (DELETE);
	else
		return (UNKNOWN);
}

uint8_t		RequestFactory::_checkHeader(void) const
{
	return (1);
}

uint8_t		RequestFactory::_checkBody(void) const
{
	return (1);
}
