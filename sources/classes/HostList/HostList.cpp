/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostList.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:32:26 by cviel             #+#    #+#             */
/*   Updated: 2026/05/20 17:34:00 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "config_file.hpp"
#include "HostList.hpp"
#include "JsonLexer.hpp"
#include "JsonObj.hpp"
#include "ObjSchema.hpp"
#include <iostream>
HostList::HostList(void)
{}

HostList::HostList(HostList const& other) :
    _map(other._map)
{}

HostList::~HostList()
{}

HostList    HostList::build(char const* filename)
{
	std::ifstream	config_file(filename);

	if (config_file.fail())
		throw std::runtime_error("File stream failed to properly setup");

	std::stringstream	input_stream;

	input_stream << config_file.rdbuf();

	JsonLexer						lexer(input_stream.str());
	std::map<std::string, JsonObj>	obj_map;

	while (!lexer.empty())
	{
		if (lexer.peekType() != JsonLexer::STRING)
			throw std::invalid_argument("Invalid key");
		
		std::string	key;

		key = lexer.popToken();
		if (lexer.peekType() != JsonLexer::COLON)
			throw std::invalid_argument("Missing ':' separator between key and value");
		lexer.popToken();

		JsonObj	val(lexer);

		if (obj_map.insert(std::pair<std::string, JsonObj>(key, val)).second == false)
			throw std::logic_error("Key '" + key + "' already exists");
	}

	ObjSchema	host_schema(HOST_KEY, true, false);
	
	host_schema_builder(host_schema);
	host_schema.validate(obj_map);

	HostList										host_list;
	std::map<std::string, JsonObj>::const_iterator	obj_map_it = obj_map.find(HOST_KEY);
	
	if (obj_map_it->second.getType() == JsonObj::ARRAY)
	{
		for (std::vector<JsonObj>::const_iterator it = obj_map_it->second.getArray().begin(); it != obj_map_it->second.getArray().end(); ++it)
		{
			addHost(VirtualHost::build(it->getSubObj()), host_list._map);
		}
	}
	else
		addHost(VirtualHost::build(obj_map_it->second.getSubObj()), host_list._map);
	return (host_list);
}

void	HostList::addHost(std::pair<uint16_t, VirtualHost> const& vhost, std::map<uint16_t, std::vector<VirtualHost> >& host_map)
{
	std::map<uint16_t, std::vector<VirtualHost> >::iterator	host_map_it = host_map.find(vhost.first);

	if (host_map_it == host_map.end())
	{
		std::vector<VirtualHost>	host_vec;

		host_vec.push_back(vhost.second);
		host_map.insert(std::pair<uint16_t, std::vector<VirtualHost> >(vhost.first, host_vec));
		return ;
	}
	for (std::vector<VirtualHost>::const_iterator map_vh_it = host_map_it->second.begin(); map_vh_it != host_map_it->second.end(); ++map_vh_it)
	{
		for (std::vector<std::string>::const_iterator map_name_it = map_vh_it->getName().begin(); map_name_it != map_vh_it->getName().end(); ++map_name_it)
		{
			for (std::vector<std::string>::const_iterator vh_name_it = vhost.second.getName().begin(); vh_name_it != vhost.second.getName().end(); ++vh_name_it)
			{
				if (*map_name_it == *vh_name_it)
					throw std::invalid_argument("Corresponding names and port for two different servers");
			}
		}
	}
	host_map_it->second.push_back(vhost.second);
}