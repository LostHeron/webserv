/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostBuilder.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:06:22 by cviel             #+#    #+#             */
/*   Updated: 2026/06/01 19:42:54 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include "config_file.hpp"
#include "VHostBuilder.hpp"
#include "JsonObj.hpp"
#include "VirtualHost.hpp"

std::pair<uint16_t, VirtualHost>	VHostBuilder::jsonBuild(std::map<std::string, JsonObj> const& obj_map)
{
	VirtualHost	host;
	
	for (std::map<std::string, std::pair<void (*)(JsonObj const&), void (*)(void)> >::const_iterator process_it = VHostBuilder::_processMap.begin(); process_it != VHostBuilder::_processMap.end(); ++process_it)
	{
		if (process_it->first != HOST_PORT_KEY)
		{
			std::map<std::string, JsonObj>::const_iterator	obj_it = obj_map.find(process_it->first);
			
			if (obj_it != obj_map.end())
			{
				if (obj_it->second.getType() == JsonObj::ARRAY)
				{
					for (std::vector<JsonObj>::const_iterator it = obj_it->second.getArray().begin(); it != obj_it->second.getArray().end(); ++it)
					{
						(*process_it->second.first)(*it);
					}
				}
				else
					(*process_it->second.first)(obj_it->second);
			}
			else
				(*process_it->second.second)();
			
		}
	}
	return (std::pair<uint16_t, VirtualHost>(obj_map.find(HOST_PORT_KEY)->second.getInt(), host));
}
