/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:06:22 by cviel             #+#    #+#             */
/*   Updated: 2026/07/01 19:15:31 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "config_file.hpp"
#include "VHostParser.hpp"
#include "JsonObj.hpp"
#include "VirtualHost.hpp"
#include "ObjSchema.hpp"

std::map<uint16_t, std::vector<VirtualHost::s_config> >	VHostParser::buildFromJson(JsonObj::SubObj const& host_map)
{
	ObjSchema	host_schema(HOST_KEY, true, true);

	host_schema_builder(host_schema);
	host_schema.validate(host_map);
	
	std::map<std::string, s_setter<VirtualHost::s_config> >	host_dispatcher = buildHostDispatcher();
	std::map<uint16_t, std::vector<VirtualHost::s_config> >	host_conf_map;
	JsonObj const&											host_obj = host_map.find(HOST_KEY)->second;
	
	if (host_obj.getType() == JsonObj::ARRAY)
	{
		for (JsonObj::Array::const_iterator	obj_it = host_obj.getVal<JsonObj::Array>().begin(); obj_it != host_obj.getVal<JsonObj::Array>().end(); ++obj_it)
		{
			std::map<int64_t, VirtualHost::s_config>	conf_map = VHostParser::dispatchJson<int64_t, VirtualHost::s_config>(obj_it->getVal<JsonObj::SubObj>(), host_dispatcher, HOST_PORT_KEY);

			VHostParser::addVHostConf(conf_map, host_conf_map);
		}
	}
	else
	{
		std::map<int64_t, VirtualHost::s_config>	conf_map = VHostParser::dispatchJson<int64_t, VirtualHost::s_config>(host_obj.getVal<JsonObj::SubObj>(), host_dispatcher, HOST_PORT_KEY);
			
		VHostParser::addVHostConf(conf_map, host_conf_map);
	}
	return (host_conf_map);
}

template <typename Key, typename Val>
std::map<Key, Val>	VHostParser::dispatchJson(JsonObj::SubObj const& obj_map, std::map<std::string, s_setter<Val> > const& dispatch_table, std::string const& key_name)
{	
	Val	output_val;
	
	for (typename std::map<std::string, s_setter<Val> >::const_iterator dispatch_it = dispatch_table.begin(); dispatch_it != dispatch_table.end(); ++dispatch_it)
	{
		if (dispatch_it->first != key_name)
		{
			JsonObj::SubObj::const_iterator	obj_it = obj_map.find(dispatch_it->first);
			
			if (obj_it != obj_map.end())
			{
				if (obj_it->second.getType() == JsonObj::ARRAY)
				{
					for (JsonObj::Array::const_iterator it = obj_it->second.getVal<JsonObj::Array>().begin(); it != obj_it->second.getVal<JsonObj::Array>().end(); ++it)
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
					(*dispatch_it->second.setDef)(output_val);
			}
			
		}
	}

	JsonObj::SubObj::const_iterator	obj_it = obj_map.find(key_name);
	std::map<Key, Val>				output_map;
	
	if (obj_it == obj_map.end())
		throw std::logic_error("Key name not found when trying to parse config");
	if (obj_it->second.getType() == JsonObj::ARRAY)
	{
		for (JsonObj::Array::const_iterator it = obj_it->second.getVal<JsonObj::Array>().begin(); it != obj_it->second.getVal<JsonObj::Array>().end(); ++it)
		{
			Key	output_key;
			
			output_key = it->getVal<Key>();
			if (output_map.insert(std::pair<Key, Val>(output_key, output_val)).second == false)
				throw std::logic_error("Dispatcher has encountered the same key twice");
		}
	}
	else
	{
		output_map.insert(std::pair<Key, Val>(obj_it->second.getVal<Key>(), output_val));
	}
	return (output_map);
}

std::map<std::string, VHostParser::s_setter<VirtualHost::s_config> >	VHostParser::buildHostDispatcher(void)
{
	std::map<std::string, VHostParser::s_setter<VirtualHost::s_config> >	dispatch_map;
	VHostParser::s_setter<VirtualHost::s_config>							setters;
	
	setters.setVal = setHostName;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_NAME_KEY, setters));
	setters.setVal = setHostRoot;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_ROOT_KEY, setters));
	setters.setVal = setHostIndex;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_INDEX_KEY, setters));
	setters.setVal = setHostMaxBody;
	setters.setDef = setHostDefMaxBody;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_BODY_SIZE_KEY, setters));
	setters.setVal = setHostInterface;
	setters.setDef = setHostDefInterface;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_INTERFACE_KEY, setters));
	setters.setVal = setHostDirList;
	setters.setDef = setHostDefDirList;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_DIR_LIST_KEY, setters));
	setters.setVal = setHostAllowedRequest;
	setters.setDef = setHostDefAllowedRequest;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_ALLOWED_REQUEST_KEY, setters));
	setters.setVal = setHostError;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_ERROR_KEY, setters));
	setters.setVal = setHostLocation;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_LOCATION_KEY, setters));
	setters.setVal = setHostCgi;
	setters.setDef = setHostDefCgi;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_CGI_KEY, setters));
	setters.setVal = setHostCgiExt;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_CGI_EXT_KEY, setters));
	setters.setVal = setHostUploadPath;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::s_config> >(HOST_UPLOAD_KEY, setters));
	return (dispatch_map);
}

std::map<std::string, VHostParser::s_setter<VirtualHost::Location::s_config> >	VHostParser::buildLocDispatcher(void)
{
	std::map<std::string, VHostParser::s_setter<VirtualHost::Location::s_config> >	dispatch_map;
	VHostParser::s_setter<VirtualHost::Location::s_config>							setters;
	
	setters.setVal = setLocAlias;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_ALIAS_KEY, setters));
	setters.setVal = setLocRedir;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_REDIRECTION_KEY, setters));
	setters.setVal = setLocIndex;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_INDEX_KEY, setters));
	setters.setVal = setLocMaxBody;
	setters.setDef = setLocDefMaxBody;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_INDEX_KEY, setters));
	setters.setVal = setLocDirList;
	setters.setDef = setLocDefDirList;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_DIR_LIST_KEY, setters));
	setters.setVal = setLocAllowedRequest;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_ALLOWED_REQUEST_KEY, setters));
	setters.setVal = setLocCgi;
	setters.setDef = setLocDefCgi;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_CGI_KEY, setters));
	setters.setVal = setLocCgiExt;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_CGI_EXT_KEY, setters));
	setters.setVal = setLocUploadPath;
	setters.setDef = NULL;
	dispatch_map.insert(std::pair<std::string, s_setter<VirtualHost::Location::s_config> >(LOC_UPLOAD_KEY, setters));
	return (dispatch_map);
}

void	VHostParser::addVHostConf(std::map<int64_t, VirtualHost::s_config> const& conf_map, std::map<uint16_t, std::vector<VirtualHost::s_config> >& host_conf_map)
{
	for (std::map<int64_t, VirtualHost::s_config>::const_iterator conf_it = conf_map.begin(); conf_it != conf_map.end(); ++conf_it)
	{
		std::map<uint16_t, std::vector<VirtualHost::s_config> >::iterator	host_conf_it = host_conf_map.find(conf_it->first);
		
		if (host_conf_it == host_conf_map.end())
		{
			std::vector<VirtualHost::s_config>	conf_vec;

			conf_vec.push_back(conf_it->second);
			host_conf_map.insert(std::pair<uint16_t, std::vector<VirtualHost::s_config> >(conf_it->first, conf_vec));
		}
		else
			host_conf_it->second.push_back(conf_it->second);
	}
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
	host_config.maxBodySize = max_body.getVal<int64_t>();	
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
	if (host_config.error.insert(std::pair<int, std::string>(error.getVal<JsonObj::SubObj>().find(ERROR_CODE_KEY)->second.getVal<int64_t>(), error.getVal<JsonObj::SubObj>().find(ERROR_PAGE_KEY)->second.getVal<std::string>())).second == false)
		throw std::logic_error(std::string(ERROR_CODE_KEY) + std::string(" duplicate"));
}

void	VHostParser::setHostLocation(JsonObj const& location, VirtualHost::s_config& host_config)
{
	std::map<std::string, s_setter<VirtualHost::Location::s_config> >	loc_dispatcher = VHostParser::buildLocDispatcher();
	std::map<std::string, VirtualHost::Location::s_config>				loc_map = VHostParser::dispatchJson<std::string, VirtualHost::Location::s_config>(location.getVal<JsonObj::SubObj>(), loc_dispatcher, LOC_NAME_KEY);

	for (std::map<std::string, VirtualHost::Location::s_config>::const_iterator it = loc_map.begin(); it != loc_map.end(); ++it)
	{
		host_config.location.insert(std::pair<std::string, VirtualHost::Location>(it->first, VirtualHost::Location(it->second)));
	}
}

void	VHostParser::setHostCgi(JsonObj const& cgi, VirtualHost::s_config& host_config)
{
	host_config.cgi = cgi.getVal<bool>();
}

void	VHostParser::setHostCgiExt(JsonObj const& cgi_ext, VirtualHost::s_config& host_config)
{
	if (VHostParser::checkDuplicates<std::string>(cgi_ext.getVal<std::string>(), host_config.cgiExt))
		throw std::logic_error(std::string(HOST_CGI_EXT_KEY) + std::string(" duplicate"));
	host_config.cgiExt.push_back(cgi_ext.getVal<std::string>());
}

void	VHostParser::setHostUploadPath(JsonObj const& upload_path, VirtualHost::s_config& host_config)
{
	host_config.uploadPath = upload_path.getVal<std::string>();
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

void	VHostParser::setLocMaxBody(JsonObj const& max_body, VirtualHost::Location::s_config& loc_config)
{
	loc_config.bodySizeInput = true;
	loc_config.maxBodySize = max_body.getVal<int64_t>();
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
	if (VHostParser::checkDuplicates<std::string>(cgi_ext.getVal<std::string>(), loc_config.cgiExt))
		throw std::logic_error(std::string(HOST_CGI_EXT_KEY) + std::string(" duplicate"));
	loc_config.cgiExt.push_back(cgi_ext.getVal<std::string>());
}

void	VHostParser::setLocUploadPath(JsonObj const& upload_path, VirtualHost::Location::s_config& loc_config)
{
	loc_config.uploadPath = upload_path.getVal<std::string>();
}

void	VHostParser::setHostDefMaxBody(VirtualHost::s_config& host_config)
{
	host_config.maxBodySize = MAX_BODY_SIZE;
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

void	VHostParser::setLocDefMaxBody(VirtualHost::Location::s_config& loc_config)
{
	loc_config.bodySizeInput = false;
	loc_config.maxBodySize = 0;
}

void	VHostParser::setLocDefDirList(VirtualHost::Location::s_config& loc_config)
{
	loc_config.allowDirList = false;
}

void	VHostParser::setLocDefCgi(VirtualHost::Location::s_config& loc_config)
{
	loc_config.cgi = false;
}
