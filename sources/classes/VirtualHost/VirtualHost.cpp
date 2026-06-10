/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/06/10 19:49:08 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "VirtualHost.hpp"
#include "JsonObj.hpp"
#include "config_file.hpp"

VirtualHost::VirtualHost(VirtualHost::s_config const& conf) :
	_conf(conf)
{
	if (this->_conf.name.empty() || this->_conf.root.empty())
		throw std::logic_error("Trying to build a virtual host with empty name and/or empty root : stop trying to setup an host yourself ! Use the intended pipeline !");
}

VirtualHost::VirtualHost(VirtualHost const& other) :
	_conf(other._conf)
{}

VirtualHost::~VirtualHost()
{}

std::vector<std::string> const&	VirtualHost::getName(void) const
{
	return (this->_conf.name);		
}

bool	VirtualHost::InterfaceAllowed(uint32_t interface) const
{
	for (std::vector<VirtualHost::s_ip_range>::const_iterator it = this->_conf.allowedInterface.begin(); it != this->_conf.allowedInterface.end(); ++it)
	{
		if (interface >= it->min && interface <= it->max)
			return (true);
	}
	return (false);
}

uint64_t	VirtualHost::getBodySize(void) const
{
	return (this->_conf.max_body_size);
}

std::pair<bool, std::string>	VirtualHost::getError(int err_code) const
{
	std::map<int, std::string>::const_iterator	err_it = this->_conf.error.find(err_code);
	
	if (err_it == this->_conf.error.end())
		return (std::pair<bool, std::string>(false, std::string()));
	return (std::pair<bool, std::string>(true, err_it->second));
}

VirtualHost::Location::Location(Location const& other) :
	_conf(other._conf)
{}

VirtualHost::Location::~Location()
{}
