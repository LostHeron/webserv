/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:56:58 by cviel             #+#    #+#             */
/*   Updated: 2026/06/04 21:45:56 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VHOSTPARSER_HPP
# define VHOSTPARSER_HPP

# include <stdint.h>
# include <string>
# include <map>
# include "JsonObj.hpp"
# include "VirtualHost.hpp"

class VHostParser
{
	public:

		static std::pair<uint16_t, VirtualHost::s_config>	buildFromJson(std::map<std::string, JsonObj> const& obj_map);

	private:
	
		typedef void (*setVal)(JsonObj const&, VirtualHost::s_config&);
		typedef void (*setDef)(void);
		typedef std::pair<setVal, setDef> setter; 
	
		static const	std::map<std::string, setter>	_dispatch;

		template <typename T>
		static bool	checkDuplicates(T const& val, std::vector<T> const& vec);
		
		static VirtualHost::s_ip_range	buildInterfaceRange(std::string const& ips_str);
		static uint32_t					buildInterface(std::string const& ip_str);
		static VirtualHost::Location	buildLocation(std::map<std::string, JsonObj> const& loc_map);

		static void	setName(JsonObj const& name, VirtualHost::s_config& config);
		static void	setRoot(JsonObj const& root, VirtualHost::s_config& config);
		static void	setIndex(JsonObj const& index, VirtualHost::s_config& config);
		static void	setMaxBody(JsonObj const& max_body, VirtualHost::s_config& config);
		static void	setInterface(JsonObj const& interface, VirtualHost::s_config& config);
		static void	setDirList(JsonObj const& dir_list, VirtualHost::s_config& config);
		static void	setAllowedRequest(JsonObj const& allowed_request, VirtualHost::s_config& config);
		static void	setError(JsonObj const& error, VirtualHost::s_config& config);
		static void	setLocation(JsonObj const& location, VirtualHost::s_config& config);
		static void	setCgi(JsonObj const& cgi, VirtualHost::s_config& config);
};

#endif // VHOSTPARSER_HPP