/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostList.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:32:26 by cviel             #+#    #+#             */
/*   Updated: 2026/06/26 17:46:37 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "config_file.hpp"
#include "VHostList.hpp"
#include "JsonLexer.hpp"
#include "JsonObj.hpp"
#include "VHostParser.hpp"

VHostList::VHostList(void)
{}

VHostList::VHostList(VHostList const& other) :
    _hosts(other._hosts)
{}

VHostList::~VHostList()
{}

VHostList    VHostList::build(char const* filename)
{
	std::ifstream	config_file(filename);

	if (config_file.fail())
		throw std::runtime_error("could not open file: '" + std::string(filename) + "'");

	std::stringstream	input_stream;

	input_stream << config_file.rdbuf();

	JsonLexer		lexer(input_stream.str());
	JsonObj::SubObj	obj_map;

	while (!lexer.empty())
	{
		if (lexer.peekType() != JsonLexer::STRING)
			throw std::invalid_argument("Invalid key");
		
		std::string	key;

		key = lexer.popToken();
		if (lexer.peekType() != JsonLexer::COLON)
			throw std::invalid_argument("Missing ':' separator between key and value");
		if (key != HOST_KEY)
			throw std::invalid_argument("Unexpected key : " + key);
		lexer.popToken();

		JsonObj	val(lexer);

		if (obj_map.insert(std::pair<std::string, JsonObj>(key, val)).second == false)
			throw std::logic_error("Key '" + key + "' already exists");
	}

	std::map<uint16_t, std::vector<VirtualHost::s_config> >	host_conf_map = VHostParser::buildFromJson(obj_map);
	VHostList												host_list;

	for (std::map<uint16_t, std::vector<VirtualHost::s_config> >::const_iterator host_conf_it = host_conf_map.begin(); host_conf_it != host_conf_map.end(); ++host_conf_it)
	{
		for (std::vector<VirtualHost::s_config>::const_iterator conf_it = host_conf_it->second.begin(); conf_it != host_conf_it->second.end(); ++conf_it)
		{
			std::pair<uint16_t, VirtualHost>	host_pair(host_conf_it->first, VirtualHost(*conf_it));

			VHostList::addHost(host_pair, host_list._hosts);
		}
	}
	return (host_list);
}

std::vector<uint16_t>	VHostList::getPort(void) const
{
	std::vector<uint16_t>	port_vec;
	
	for (HostMap::const_iterator it = this->_hosts.begin(); it != this->_hosts.end(); ++it)
	{
		port_vec.push_back(it->first);
	}
	return (port_vec);
}

VirtualHost const&	VHostList::getHost(uint16_t port, std::string const& name) const
{
	HostMap::const_iterator	host_list_it = this->_hosts.find(port);

	if (host_list_it == this->_hosts.end())
	{
		std::stringstream	stream;

		stream << port;
		throw std::runtime_error("No match for port " + stream.str());
	}
	for (std::vector<VirtualHost>::const_iterator vhost_it = host_list_it->second.begin(); vhost_it != host_list_it->second.end(); ++vhost_it)
	{
		for (std::vector<std::string>::const_iterator name_it = vhost_it->getName().begin(); name_it != vhost_it->getName().end(); ++name_it)
		{
			if (*name_it == name)
				return (*vhost_it);
		}
	}
	return (*(host_list_it->second.begin()));
}

void	VHostList::addHost(std::pair<uint16_t, VirtualHost> const& vhost, HostMap& host_map)
{
	std::map<uint16_t, std::vector<VirtualHost> >::iterator	host_map_it = host_map.find(vhost.first);

	if (host_map_it == host_map.end())
	{
		std::vector<VirtualHost>	host_vec;

		host_vec.push_back(vhost.second);
		host_map.insert(std::pair<uint16_t, std::vector<VirtualHost> >(vhost.first, host_vec));
		return ;
	}
	for (std::vector<VirtualHost>::const_iterator host_it = host_map_it->second.begin(); host_it != host_map_it->second.end(); ++host_it)
	{
		for (std::vector<std::string>::const_iterator host_name_it = host_it->getName().begin(); host_name_it != host_it->getName().end(); ++host_name_it)
		{
			for (std::vector<std::string>::const_iterator pair_name_it = vhost.second.getName().begin(); pair_name_it != vhost.second.getName().end(); ++pair_name_it)
			{
				if (*host_name_it == *pair_name_it)
					throw std::invalid_argument("Corresponding names and port for two different servers");
			}
		}
	}
	host_map_it->second.push_back(vhost.second);
}
