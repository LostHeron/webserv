/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/06/04 21:20:02 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "VirtualHost.hpp"
#include "JsonObj.hpp"
#include "config_file.hpp"

VirtualHost::VirtualHost(VirtualHost const& other) :
	_conf(other._conf)
{}

VirtualHost::~VirtualHost()
{}

std::vector<std::string> const&	VirtualHost::getName(void) const
{
	return (this->_conf.name);		
}

std::pair<std::string, bool>	VirtualHost::getPathReq(std::string const& uri, std::string const& req) const
{
	std::pair<std::string, bool>	path_req_pair(this->_conf.root + uri, false);

	for (std::vector<std::string>::const_iterator it = this->_conf.allowedRequest.begin(); it != this->_conf.allowedRequest.end(); ++it)
	{
		if (*it == req)
		{
			path_req_pair.second = true;
			break ;
		}
	}
	return (path_req_pair);
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
