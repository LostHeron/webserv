/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 18:10:11 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestFactory.hpp"

// Static init =================================================================
AFactory<ARequest>::_constructorsArray[TYPES_QTY] =
{
	&newElement<GETReq>,
	&newElement<POSTReq>,
	&newElement<DELETEReq>
};

AFactory<ARequest>::_testsArray[] =
{
	&_isGet,
	&_isPost,
	&_isDelete,
	&_checkHeader,
	&_checkBody,
};


// Constructors/Destructor =====================================================
RequestFactory::RequestFactory(void):
	AFactory<ARequest>() {}

RequestFactory::RequestFactory(const RequestFactory &cpy):
	AFactory<ARequest>(cpy) {}

RequestFactory::~RequestFactory(void) {}


// Ops overloading =============================================================
RequestFactory	&RequestFactory::operator=(const RequestFactory &assign) { (void) assign; return (*this); }


// Member functions =============================================================
void			RequestFactory::execute(void)
{
	std::cout << "Cannot execute Factory class." << std::endl;
}

uint8_t			RequestFactory::determineElement(void) const
{

	if (!this->_testsStatus & (1 << CHECKHEADER))
		return (UNKNOWN);

	int	i;

	for (i = 0; i < TYPE_QTY; ++i)
	{
		if (this->_testsStatus & (1 << i))
		{
			if (i == ISPOST && this->_testsStatus & (1 << CHECKBODY))
				break;
			else
				return (UNKNOWN);
		}
		else if (i == TYPES_QTY - 1  && !this->_testsStatus & (1 << i))
			return (UNKNOWN);
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
