/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/05/19 16:08:53 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "VirtualHost.hpp"
#include "JsonObj.hpp"
#include "config_file.hpp"

VirtualHost::VirtualHost(void) :
	_index("index.html"),
	_max_body_size(MAX_BODY_SIZE),
	_allowDirList(false)
{}

VirtualHost::VirtualHost(VirtualHost const& other) :
	_name(other._name),
	_root(other._root),
	_index(other._index),
	_max_body_size(other._max_body_size),
	_allowedInterface(other._allowedInterface),
	_allowedRequest(other._allowedRequest),
	_allowDirList(other._allowDirList),
	_error(other._error),
	_location(other._location)
{}

VirtualHost::~VirtualHost()
{}

std::pair<u_int16_t, VirtualHost>	VirtualHost::build(std::map<std::string, JsonObj> obj_map)
{
	u_int16_t	port = obj_map.find(HOST_PORT_KEY)->second.getInt();
	VirtualHost	host;
	std::map<std::string, JsonObj>::const_iterator	obj_it = obj_map.find(HOST_NAME_KEY);
	
	if (obj_it->second.getType() == JsonObj::ARRAY)
	{
		for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
		{
			if (VirtualHost::checkDuplicates<std::string>(it->getString(), host._name) == true)
				throw std::logic_error(std::string(HOST_NAME_KEY) + std::string(" array has duplicates"));
			host._name.push_back(it->getString());
		}
	}
	else
		host._name.push_back(obj_it->second.getString());
	host._root = obj_map.find(HOST_ROOT_KEY)->second.getString();
	obj_it = obj_map.find(HOST_INDEX_KEY);
	if (obj_it != obj_map.end())
		host._index = obj_it->second.getString();
	obj_it = obj_map.find(HOST_BODY_SIZE_KEY);
	if (obj_it != obj_map.end())
		host._max_body_size = obj_it->second.getInt();
	obj_it = obj_map.find(HOST_INTERFACE_KEY);
	if (obj_it != obj_map.end())
	{
		if (obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
			{
				VirtualHost::s_ip_range	ip_range = VirtualHost::buildInterfaceRange(it->getString());
				
				if (VirtualHost::checkDuplicates<VirtualHost::s_ip_range>(ip_range, host._allowedInterface) == true)
					throw std::logic_error(std::string(HOST_INTERFACE_KEY) + std::string(" array has duplicates"));
				host._allowedInterface.push_back(ip_range);
			}
		}
		else
			host._allowedInterface.push_back(VirtualHost::buildInterfaceRange(obj_it->second.getString()));
	}
	else
		host._allowedInterface.push_back(buildInterfaceRange("0.0.0.0:255.255.255.255"));
	obj_it = obj_map.find(HOST_ALLOWED_REQUEST_KEY);
	if (obj_it != obj_map.end())
	{
		if (obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
			{
				if (VirtualHost::checkDuplicates<std::string>(it->getString(), host._allowedRequest) == true)
					throw std::logic_error(std::string(HOST_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
				host._allowedRequest.push_back(it->getString());
			}
		}
		else
			host._allowedRequest.push_back(obj_it->second.getString());
	}
	else
		host._allowedRequest.push_back("GET");
	obj_it = obj_map.find(HOST_DIR_LIST_KEY);
	if (obj_it != obj_map.end())
		host._allowDirList = obj_it->second.getBool();
	obj_it = obj_map.find(HOST_ERROR_KEY);
	if (obj_it != obj_map.end())
	{
		if (obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
			{
				VirtualHost::addErrorPage(it->getSubObj(), host._error);
			}
		}
		else
			VirtualHost::addErrorPage(obj_it->second.getSubObj(), host._error);
	}
	obj_it = obj_map.find(HOST_LOCATION_KEY);
	if (obj_it != obj_map.end())
	{
		if (obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
			{
				VirtualHost::addLocation(it->getSubObj(), host._location);
			}
		}
		else
			VirtualHost::addLocation(obj_it->second.getSubObj(), host._location);
	}
	obj_it = obj_map.find(HOST_CGI_KEY);
	if (obj_it != obj_map.end())
	{
		if (obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
			{
				VirtualHost::addCgi(it->getSubObj(), host._cgi);
			}
		}
		else
			VirtualHost::addCgi(obj_it->second.getSubObj(), host._cgi);
	}
	return (std::pair<u_int16_t, VirtualHost>(obj_map.find(HOST_PORT_KEY)->second.getInt(), host));
}

template <typename T>
bool	VirtualHost::checkDuplicates(T const& val, std::vector<T> const& vec)
{
	for (std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (val == *it)
			return (true);
	}
	return (false);
}

VirtualHost::s_ip_range	VirtualHost::buildInterfaceRange(std::string const& interfaces)
{
	VirtualHost::s_ip_range	ip_range;
	std::string::size_type	colon_pos = interfaces.find(':');

	if (colon_pos != std::string::npos)
	{
		ip_range.min = buildInterface(interfaces.substr(0, colon_pos));
		ip_range.max = buildInterface(interfaces.substr(colon_pos + 1));
		if (ip_range.min > ip_range.max)
			throw std::logic_error("Interface range is incorrect : 'first:last' has higher first than last");
		return (ip_range);
	}
	ip_range.min = buildInterface(interfaces);
	ip_range.max = ip_range.min;
	return (ip_range);
}

u_int32_t	VirtualHost::buildInterface(std::string const& interface)
{
	u_int32_t			interface_bits = 0;
	std::stringstream	interface_stream(interface);

	for (unsigned int i = 0; i < 4; ++i)
	{
		u_int8_t	interface_part;
		
		interface_stream >> interface_part;
		if (i < 3)
		{
			interface_bits <<= 8;
			interface_bits += interface_part;
			interface_stream.ignore();
		}
	}
}

void	VirtualHost::addErrorPage(std::map<std::string, JsonObj> const& error, std::map<int, std::string>& host_error)
{
	if (host_error.insert(std::pair<int, std::string>(error.find(ERROR_CODE_KEY)->second.getInt(), error.find(ERROR_PAGE_KEY)->second.getString())).second == false)
		throw std::logic_error("Error code duplicate");
}

void	VirtualHost::addLocation(std::map<std::string, JsonObj> const& location, std::map<std::string, VirtualHost::Location>& host_location)
{
	if (host_location.insert(VirtualHost::Location::build(location)).second == false)
		throw std::logic_error("Location duplicate");
}

void	VirtualHost::addCgi(std::map<std::string, JsonObj> const& cgi, std::map<std::string, std::string>& host_cgi)
{
	if (host_cgi.insert(std::pair<std::string, std::string>(cgi.find(CGI_EXTENSION_KEY)->second.getString(), cgi.find(CGI_EXEC_KEY)->second.getString())).second == false)
		throw std::logic_error("CGI duplicate");
}

std::vector<std::string> const&	VirtualHost::getName(void) const
{
	return (this->_name);		
}

std::pair<std::string, VirtualHost::Location>	VirtualHost::Location::build(std::map<std::string, JsonObj> const& loc_obj_map)
{
	Location	loc;
	std::map<std::string, JsonObj>::const_iterator	loc_obj_it = loc_obj_map.find(LOC_ROOT_KEY);

	loc._root = loc_obj_it->second.getString();
	loc_obj_it = loc_obj_map.find(LOC_INDEX_KEY);
	if (loc_obj_it != loc_obj_map.end());
		loc._index = loc_obj_it->second.getString();
	loc_obj_it = loc_obj_map.find(LOC_ALLOWED_REQUEST_KEY);
	if (loc_obj_it != loc_obj_map.end())
	{
		if (loc_obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = loc_obj_it->second.getArray().begin(); it != loc_obj_it->second.getArray().end(); ++it)
			{
				if (VirtualHost::checkDuplicates<std::string>(it->getString(), loc._allowedRequest) == true)
					throw std::logic_error(std::string(LOC_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
				loc._allowedRequest.push_back(it->getString());
			}
		}
		else
			loc._allowedRequest.push_back(loc_obj_it->second.getString());
	}
	loc_obj_it = loc_obj_map.find(LOC_DIR_LIST_KEY);
	if (loc_obj_it != loc_obj_map.end())
		loc._allowDirList = loc_obj_it->second.getBool();
	loc_obj_it = loc_obj_map.find(LOC_REDIRECTION_KEY);
	if (loc_obj_it != loc_obj_map.end())
	{
		if (loc_obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = loc_obj_it->second.getArray().begin(); it != loc_obj_it->second.getArray().end(); ++it)
			{
				Location::s_redir redir = Location::buildRedir(it->getString());
				
				if (VirtualHost::checkDuplicates<Location::s_redir>(redir, loc._redirection) == true)
					throw std::logic_error(std::string(LOC_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
				loc._redirection.push_back(redir);
			}
		}
		else
			loc._redirection.push_back(Location::buildRedir(loc_obj_it->second.getString()));
	}
	loc_obj_it = loc_obj_map.find(LOC_CGI_KEY);
	if (loc_obj_it != loc_obj_map.end())
	{
		if (loc_obj_it->second.getType() == JsonObj::ARRAY)
		{
			for (std::vector<JsonObj>::const_iterator it = loc_obj_it->second.getArray().begin(); it != loc_obj_it->second.getArray().end(); ++it)
			{
				addCgi(it->getSubObj(), loc._cgi);
			}
		}
		else
			addCgi(loc_obj_it->second.getSubObj(), loc._cgi);
	}
	return (std::pair<std::string, Location>(loc_obj_map.find(LOC_NAME_KEY)->second.getString(), loc));
}

VirtualHost::Location::s_redir	VirtualHost::Location::buildRedir(std::string const& redirection)
{
	Location::s_redir		redir;
	std::string::size_type	colon_pos = redirection.find(':');

	redir.from = redirection.substr(0, colon_pos);
	redir.to = redirection.substr(colon_pos + 1);
	return (redir);
}