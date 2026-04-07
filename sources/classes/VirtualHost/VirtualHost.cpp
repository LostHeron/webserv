/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/04/03 18:50:28 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualHost.hpp"

VirtualHost::VirtualHost(void) :
	_hosts(),
	_root(),
	_index(),
	_allowedReqBitfield(0),
	_errorPage(),
	// _streambufNull(),
	// _streamNull(&_streambufNull),
	// _successLogs(&_streamNull),
	// _errorLogs(&_streamNull),
	_locations()
{}

VirtualHost::VirtualHost(VirtualHost const& other) :
	_hosts(other._hosts),
	_root(other._root),
	_index(other._index),
	_allowedReqBitfield(other._allowedReqBitfield),
	_errorPage(other._errorPage),
	// _streambufNull(),
	// _streamNull(&_streambufNull),
	// _successLogs(&_streamNull),
	// _errorLogs(&_streamNull),
	_locations(other._locations)
{}

VirtualHost::~VirtualHost()
{}

VirtualHost&	VirtualHost::operator=(VirtualHost const& other)
{
	if (this != &other)
	{
		this->_hosts = other._hosts;
		this->_root = other._root;
		this->_index = other._index;
		this->_allowedReqBitfield = other._allowedReqBitfield;
		this->_errorPage = other._errorPage;
		this->_locations = other._locations;
	}
	return (*this);
}

