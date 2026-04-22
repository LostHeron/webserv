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

const AFactory<ARequest>::_test			RequestFactory::_testsArray[] =
{
	&RequestFactory::_isGet,
	&RequestFactory::_isPost,
	&RequestFactory::_isDelete,
	&RequestFactory::_checkHeader,
	&RequestFactory::_checkBody
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

const AFactory<ARequest>::_test		*RequestFactory::_getTests(void) const
{
	return (RequestFactory::_testsArray);
}

// Member functions =============================================================
void			RequestFactory::execute(void)
{
	std::cout << "Cannot execute Factory class." << std::endl;
}

uint8_t			RequestFactory::_determineElement(void) const
{
	if (!(this->_testsStatus & (1 << 3)))
		return (4);

	int	i;

	for (i = 0; i < 3; ++i)
	{
		if (this->_testsStatus & (1 << i))
		{
			if (i == 1 && (this->_testsStatus & (1 << 4)))
				break;
			else
				return (4);
		}
		else if (i == 3 - 1  && !(this->_testsStatus & (1 << i)))
			return (4);
	}
	return (i);
}


// Tests =======================================================================
uint8_t		RequestFactory::_isGet(void) const
{
	if (this->_type == "GET")
		return (1);
	else
		return (0);
}

uint8_t		RequestFactory::_isPost(void) const
{
	if (this->_type == "POST")
		return (1);
	else
		return (0);
}

uint8_t		RequestFactory::_isDelete(void) const
{
	if (this->_type == "DELETE")
		return (1);
	else
		return (0);
}

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
