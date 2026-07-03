/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/28 11:48:11 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestFactory.hpp"
#include "VirtualHost.hpp"

// Static init =================================================================
const AFactory<ARequest>::_constructor	RequestFactory::_constructorsArray[] =
{
	&RequestFactory::_newElement<GETReq>,
	&RequestFactory::_newElement<POSTReq>,
	&RequestFactory::_newElement<PUTReq>,
	&RequestFactory::_newElement<DELETEReq>,
	&RequestFactory::_newElement<UNKNOWNReq>
};

// Constructors/Destructor =====================================================
RequestFactory::RequestFactory(const InputSocket &IOMessage, const VirtualHost& vhost):
	ARequest(IOMessage, vhost),
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
void				RequestFactory::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	(void) resp;
	(void) uriInfo;
}

ARequest		*RequestFactory::createElement(void) const
{
	const _constructor	*constructorsArray = this->_getConstructors();
	const int			i = this->_determineElement();

	return (constructorsArray[i](*this));
}

int8_t			RequestFactory::_determineElement(void) const
{
	if (this->_method == "GET")
		return (ARequest::GET);
	else if (this->_method == "POST")
		return (ARequest::GET);
	else if (this->_method == "PUT")
		return (ARequest::PUT);
	else if (this->_method == "DELETE")
		return (ARequest::DELETE);
	else
		return (UNKNOWN);
}
