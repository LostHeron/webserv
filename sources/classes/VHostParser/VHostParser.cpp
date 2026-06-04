/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:06:22 by cviel             #+#    #+#             */
/*   Updated: 2026/06/04 21:46:10 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include <sstream>
#include "config_file.hpp"
#include "VHostParser.hpp"
#include "JsonObj.hpp"
#include "VirtualHost.hpp"

std::pair<uint16_t, VirtualHost::s_config>	VHostParser::buildFromJson(std::map<std::string, JsonObj> const& obj_map)
{	
	VirtualHost::s_config	config;
	
	for (std::map<std::string, setter>::const_iterator dispatch_it = VHostParser::_dispatch.begin(); dispatch_it != VHostParser::_dispatch.end(); ++dispatch_it)
	{
		if (dispatch_it->first != HOST_PORT_KEY)
		{
			std::map<std::string, JsonObj>::const_iterator	obj_it = obj_map.find(dispatch_it->first);
			
			if (obj_it != obj_map.end())
			{
				if (obj_it->second.getType() == JsonObj::ARRAY)
				{
					for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
					{
						(*dispatch_it->second.first)(*it, config);
					}
				}
				else
					(*dispatch_it->second.first)(obj_it->second, config);
			}
			else
			{
				if (dispatch_it->second.second != NULL)
					(*dispatch_it->second.second)();
			}
			
		}
	}
	return (std::pair<uint16_t, VirtualHost::s_config>(obj_map.find(HOST_PORT_KEY)->second.getInt(), config));
}

template <typename T>
bool	VHostParser::checkDuplicates(T const& val, std::vector<T> const& vec)
{
	for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (val == *it)
			return (true);
	}
	return (false);
}

VirtualHost::s_ip_range	VHostParser::buildInterfaceRange(std::string const& ips_str)
{
	VirtualHost::s_ip_range	ip_range;
	std::string::size_type	colon_pos = ips_str.find(':');

	if (colon_pos != std::string::npos)
	{
		ip_range.min = buildInterface(ips_str.substr(0, colon_pos));
		ip_range.max = buildInterface(ips_str.substr(colon_pos + 1));
		if (ip_range.min > ip_range.max)
			throw std::logic_error("Interface range is incorrect : 'first:last' has higher first than last");
		return (ip_range);
	}
	ip_range.min = buildInterface(ips_str);
	ip_range.max = ip_range.min;
	return (ip_range);
}

uint32_t	VHostParser::buildInterface(std::string const& ip_str)
{
	uint32_t			ip_bits = 0;
	std::stringstream	ip_stream(ip_str);

	for (unsigned int i = 0; i < 4; ++i)
	{
		uint32_t	ip_part;
		
		ip_stream >> ip_part;
		if (i < 3)
		{
			ip_bits <<= 8;
			ip_stream.ignore();
		}
		ip_bits += ip_part;
	}
	return (ip_bits);
}

void	VHostParser::setName(JsonObj const& name, VirtualHost::s_config& config)
{
	if (VHostParser::checkDuplicates<std::string>(name.getString(), config.name))
		throw std::logic_error(std::string(HOST_NAME_KEY) + std::string(" array has duplicates"));
	config.name.push_back(name.getString());
}


void	VHostParser::setRoot(JsonObj const& root, VirtualHost::s_config& config)
{
	config.root = root.getString();	
}

void	VHostParser::setIndex(JsonObj const& index, VirtualHost::s_config& config)
{
	config.index = index.getString();
}


void	VHostParser::setMaxBody(JsonObj const& max_body, VirtualHost::s_config& config)
{
	config.max_body_size = max_body.getInt();	
}

void	VHostParser::setInterface(JsonObj const& interface, VirtualHost::s_config& config)
{
	VirtualHost::s_ip_range	ip_range = VHostParser::buildInterfaceRange(interface.getString());

	if (VHostParser::checkDuplicates<VirtualHost::s_ip_range>(ip_range, config.allowedInterface))
		throw std::logic_error(std::string(HOST_INTERFACE_KEY) + std::string(" array has duplicates"));
	config.allowedInterface.push_back(ip_range);
}

void	VHostParser::setDirList(JsonObj const& dir_list, VirtualHost::s_config& config)
{
	config.allowDirList = dir_list.getBool();
}

void	VHostParser::setAllowedRequest(JsonObj const& allowed_request, VirtualHost::s_config& config)
{
	if (VHostParser::checkDuplicates<std::string>(allowed_request.getString(), config.allowedRequest))
		throw std::logic_error(std::string(HOST_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
	config.allowedRequest.push_back(allowed_request.getString());
}

void	VHostParser::setError(JsonObj const& error, VirtualHost::s_config& config)
{
	if (config.error.insert(std::pair<int, std::string>(error.getSubObj().find(ERROR_CODE_KEY)->second.getInt(), error.getSubObj().find(ERROR_PAGE_KEY)->second.getString())).second == false)
		throw std::logic_error(std::string(ERROR_CODE_KEY) + std::string(" duplicate"));
}

void	VHostParser::setLocation(JsonObj const& location, VirtualHost::s_config& config)
{
	if (config.location.insert(std::pair<std::string, VirtualHost::Location>(location.getSubObj().find(LOC_NAME_KEY)->second.getString(), VHostParser::buildLocation(location.getSubObj()))).second == false)
		throw std::logic_error("Location duplicate");
}

void	VHostParser::setCgi(JsonObj const& cgi, VirtualHost::s_config& config)
{
	config.cgi = cgi.getBool();
}
