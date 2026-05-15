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
RequestFactory::RequestFactory(const IOFd &IOMessage):
	ARequest(IOMessage),
	AFactory<ARequest>() {}

RequestFactory::RequestFactory(const RequestFactory &cpy):
	ARequest(cpy),
	AFactory<ARequest>() {}

RequestFactory::~RequestFactory(void) {}


// Ops overloading =============================================================
RequestFactory	&RequestFactory::operator=(const RequestFactory &assign) { (void) assign; return (*this); }


// Getters ======================================================================
const AFactory<ARequest>::_constructor		*RequestFactory::_getConstructors(void) const
{
	return (RequestFactory::_constructorsArray);
}

// Member functions =============================================================
Response			RequestFactory::execute(void)
{
	Response resp(-1);

	std::cout << "Cannot execute Factory class." << std::endl;

	return (resp);
}

ARequest		*RequestFactory::createElement(void) const
{
	const _constructor	*constructorsArray = this->_getConstructors();
	const int			i = this->_determineElement();

	if (i == ERROR)
		return (NULL);
	return (constructorsArray[i](*this));
}

int8_t			RequestFactory::_determineElement(void) const
{
	if (!this->_checkHeader())
		return (ERROR);

	if (this->_method == "GET")
		return (ARequest::GET);
	else if (this->_method == "POST" && this->_checkBody())
		return (ARequest::POST);
	else if (this->_method == "DELETE")
		return (ARequest::DELETE);
	else
		return (ERROR);
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
