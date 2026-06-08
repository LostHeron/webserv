/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:06:22 by cviel             #+#    #+#             */
/*   Updated: 2026/06/08 19:48:17 by cviel            ###   ########.fr       */
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

template <typename Key, typename Val>
std::vector<std::pair<Key, Val> >	VHostParser::buildFromJson(std::map<std::string, JsonObj> const& obj_map, std::map<std::string, s_setter<Val> > const& dispatch_table, std::string const& key_name)
{	
	Val	output_val;
	
	for (std::map<std::string, s_setter<Val> >::const_iterator dispatch_it = dispatch_table.begin(); dispatch_it != dispatch_table.end(); ++dispatch_it)
	{
		if (dispatch_it->first != key_name)
		{
			std::map<std::string, JsonObj>::const_iterator	obj_it = obj_map.find(dispatch_it->first);
			
			if (obj_it != obj_map.end())
			{
				if (obj_it->second.getType() == JsonObj::ARRAY)
				{
					for (std::vector<JsonObj>::const_iterator it = obj_it->second.getVal<JsonObj::Array>().begin(); it != obj_it->second.getVal<JsonObj::Array>().end(); ++it)
					{
						(*dispatch_it->second.setVal)(*it, output_val);
					}
				}
				else
					(*dispatch_it->second.setVal)(obj_it->second, output_val);
			}
			else
			{
				if (dispatch_it->second.setDef != NULL)
					(*dispatch_it->second.setDef)();
			}
			
		}
	}

	std::map<std::string, JsonObj>::const_iterator	obj_it = obj_map.find(key_name);
	std::vector<std::pair<Key, Val> >				output_vec;
	
	if (obj_it->second.getType == JsonObj::ARRAY)
	{
		for (std::vector<JsonObj>::const_iterator it = obj_it->second.getVal<JsonObj::Array>().begin(); it != obj_it->second.getVal<JsonObj::Array>().end(); ++it)
		{
			Key	output_key;
			
			output_key = it.getVal<Key>();
			output_vec.push_back(std::pair<Key, Val>(output_key, output_val));
		}
	}
	else
	{
		output_vec.push_back(std::pair<Key, Val>(obj_it->second.getVal<Key>(), ouput_val));
	}
	return (output_vec);
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

void	VHostParser::setHostName(JsonObj const& name, VirtualHost::s_config& host_config)
{
	if (VHostParser::checkDuplicates<std::string>(name.getVal<std::string>(), host_config.name))
		throw std::logic_error(std::string(HOST_NAME_KEY) + std::string(" array has duplicates"));
	host_config.name.push_back(name.getVal<std::string>());
}


void	VHostParser::setHostRoot(JsonObj const& root, VirtualHost::s_config& host_config)
{
	host_config.root = root.getVal<std::string>();	
}

void	VHostParser::setHostIndex(JsonObj const& index, VirtualHost::s_config& host_config)
{
	host_config.index = index.getVal<std::string>();
}


void	VHostParser::setHostMaxBody(JsonObj const& max_body, VirtualHost::s_config& host_config)
{
	host_config.max_body_size = max_body.getVal<int>();	
}

void	VHostParser::setHostInterface(JsonObj const& interface, VirtualHost::s_config& host_config)
{
	VirtualHost::s_ip_range	ip_range = VHostParser::buildInterfaceRange(interface.getVal<std::string>());

	if (VHostParser::checkDuplicates<VirtualHost::s_ip_range>(ip_range, host_config.allowedInterface))
		throw std::logic_error(std::string(HOST_INTERFACE_KEY) + std::string(" array has duplicates"));
	host_config.allowedInterface.push_back(ip_range);
}

void	VHostParser::setHostDirList(JsonObj const& dir_list, VirtualHost::s_config& host_config)
{
	host_config.allowDirList = dir_list.getVal<bool>();
}

void	VHostParser::setHostAllowedRequest(JsonObj const& allowed_request, VirtualHost::s_config& host_config)
{
	if (VHostParser::checkDuplicates<std::string>(allowed_request.getVal<std::string>(), host_config.allowedRequest))
		throw std::logic_error(std::string(HOST_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
	host_config.allowedRequest.push_back(allowed_request.getVal<std::string>());
}

void	VHostParser::setHostError(JsonObj const& error, VirtualHost::s_config& host_config)
{
	if (host_config.error.insert(std::pair<int, std::string>(error.getVal<JsonObj::SubObj>().find(ERROR_CODE_KEY)->second.getVal<int>(), error.getVal<JsonObj::SubObj>().find(ERROR_PAGE_KEY)->second.getVal<std::string>())).second == false)
		throw std::logic_error(std::string(ERROR_CODE_KEY) + std::string(" duplicate"));
}

void	VHostParser::setHostLocation(JsonObj const& location, VirtualHost::s_config& host_config)
{
	std::map<std::string, s_setter<VirtualHost::Location> >		loc_dispatch = VHostParser::build_loc_dispatcher();
	std::vector<std::pair<std::string, VirtualHost::Location> >	loc_vec = VHostParser::buildFromJson<std::string, VirtualHost::Location>(location.getVal<JsonObj::SubObj>(), loc_dispatch, LOC_NAME_KEY);

	for (std::vector<std::pair<std::string, VirtualHost::Location> >::const_iterator it = loc_vec.begin(); it != loc_vec.end(); ++it)
	{
		if (host_config.location.insert(*it).second == false)
			throw std::logic_error("Location duplicate");
	}
}

void	VHostParser::setHostCgi(JsonObj const& cgi, VirtualHost::s_config& host_config)
{
	host_config.cgi = cgi.getVal<bool>();
}

void	VHostParser::setHostCgiExt(JsonObj const& cgi_ext, VirtualHost::s_config& host_config)
{
	if (VHostParser::checkDuplicates<std::string>(cgi_ext.getVal<std::string>(), host_config.cgi_ext))
		throw std::logic_error(std::string(HOST_CGI_EXT_KEY) + std::string(" duplicate"));
	host_config.cgi_ext.push_back(cgi_ext.getVal<std::string>());
}

void	VHostParser::setLocAlias(JsonObj const& alias, VirtualHost::Location::s_config& loc_config)
{
	loc_config.alias = alias.getVal<std::string>();
}

void	VHostParser::setLocRedir(JsonObj const& redir, VirtualHost::Location::s_config& loc_config)
{
	loc_config.redirection = redir.getVal<std::string>();
}

void	VHostParser::setLocIndex(JsonObj const& index, VirtualHost::Location::s_config& loc_config)
{
	loc_config.index = index.getVal<std::string>();
}

void	VHostParser::setLocDirList(JsonObj const& dir_list, VirtualHost::Location::s_config& loc_config)
{
	loc_config.allowDirList = dir_list.getVal<bool>();
}

void	VHostParser::setLocAllowedRequest(JsonObj const& allowed_request, VirtualHost::Location::s_config& loc_config)
{
	if (VHostParser::checkDuplicates<std::string>(allowed_request.getVal<std::string>(), loc_config.allowedRequest))
		throw std::logic_error(std::string(HOST_ALLOWED_REQUEST_KEY) + std::string(" array has duplicates"));
	loc_config.allowedRequest.push_back(allowed_request.getVal<std::string>());	
}

void	VHostParser::setLocCgi(JsonObj const& cgi, VirtualHost::Location::s_config& loc_config)
{
	loc_config.cgi = cgi.getVal<bool>();	
}

void	VHostParser::setLocCgiExt(JsonObj const& cgi_ext, VirtualHost::Location::s_config& loc_config)
{
	if (VHostParser::checkDuplicates<std::string>(cgi_ext.getVal<std::string>(), loc_config.cgi_ext))
		throw std::logic_error(std::string(HOST_CGI_EXT_KEY) + std::string(" duplicate"));
	loc_config.cgi_ext.push_back(cgi_ext.getVal<std::string>());
}

void	VHostParser::setHostDefIndex(VirtualHost::s_config& host_config)
{
	host_config.index = std::string("index.html");
}
void	VHostParser::setHostDefMaxBody(VirtualHost::s_config& host_config)
{
	host_config.max_body_size = MAX_BODY_SIZE;
}
void	VHostParser::setHostDefInterface(VirtualHost::s_config& host_config)
{
	host_config.allowedInterface.push_back(VHostParser::buildInterfaceRange("0.0.0.0:255.255.255.255"));
}

void	VHostParser::setHostDefDirList(VirtualHost::s_config& host_config)
{
	host_config.allowDirList = false;
}

void	VHostParser::setHostDefAllowedRequest(VirtualHost::s_config& host_config)
{
	host_config.allowedRequest.push_back("GET");
}

void	VHostParser::setHostDefCgi(VirtualHost::s_config& host_config)
{
	host_config.cgi = false;
}

void	VHostParser::setLocDefIndex(VirtualHost::Location::s_config& loc_config)
{
	loc_config.index = std::string("index.html");
}

void	VHostParser::setLocDefDirList(VirtualHost::Location::s_config& loc_config)
{
	loc_config.allowDirList = false;
}

void	VHostParser::setLocDefAllowedRequest(VirtualHost::Location::s_config& loc_config)
{
	loc_config.allowedRequest.push_back("GET");
}

void	VHostParser::setLocDefCgi(VirtualHost::Location::s_config& loc_config)
{
	loc_config.cgi = false;
}
