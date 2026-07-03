/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostParser.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:56:58 by cviel             #+#    #+#             */
/*   Updated: 2026/07/01 19:02:48 by cviel            ###   ########.fr       */
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

		template <typename T>
		struct s_setter
		{
			void (*setVal)(JsonObj const&, T&);
			void (*setDef)(T&);
		};	
	
		static std::map<uint16_t, std::vector<VirtualHost::s_config> >	buildFromJson(JsonObj::SubObj const& obj_map);

	private:
	
		template <typename Key, typename Val>
		static std::map<Key, Val>	dispatchJson(JsonObj::SubObj const& obj_map, std::map<std::string, s_setter<Val> > const& dispatch_table, std::string const& key_name);
	
		template <typename T>
		static bool	checkDuplicates(T const& val, std::vector<T> const& vec);
		
		static std::map<std::string, s_setter<VirtualHost::s_config> >				buildHostDispatcher(void);
		static std::map<std::string, s_setter<VirtualHost::Location::s_config> >	buildLocDispatcher(void);

		static void	addVHostConf(std::map<int64_t, VirtualHost::s_config> const& conf_map, std::map<uint16_t, std::vector<VirtualHost::s_config> >& host_conf_map);
		
		static VirtualHost::s_ip_range	buildInterfaceRange(std::string const& ips_str);
		static uint32_t					buildInterface(std::string const& ip_str);
		
		static void	setHostName(JsonObj const& name, VirtualHost::s_config& host_config);
		static void	setHostRoot(JsonObj const& root, VirtualHost::s_config& host_config);
		static void	setHostIndex(JsonObj const& index, VirtualHost::s_config& host_config);
		static void	setHostMaxBody(JsonObj const& max_body, VirtualHost::s_config& host_config);
		static void	setHostInterface(JsonObj const& interface, VirtualHost::s_config& host_config);
		static void	setHostDirList(JsonObj const& dir_list, VirtualHost::s_config& host_config);
		static void	setHostAllowedRequest(JsonObj const& allowed_request, VirtualHost::s_config& host_config);
		static void	setHostError(JsonObj const& error, VirtualHost::s_config& host_config);
		static void	setHostLocation(JsonObj const& location, VirtualHost::s_config& host_config);
		static void	setHostCgi(JsonObj const& cgi, VirtualHost::s_config& host_config);
		static void	setHostCgiExt(JsonObj const& cgi_ext, VirtualHost::s_config& host_config);
		static void	setHostUploadPath(JsonObj const& upload_path, VirtualHost::s_config& host_config);

		static void	setLocAlias(JsonObj const& alias, VirtualHost::Location::s_config& loc_config);
		static void	setLocRedir(JsonObj const& redir, VirtualHost::Location::s_config& loc_config);
		static void	setLocIndex(JsonObj const& index, VirtualHost::Location::s_config& loc_config);
		static void	setLocMaxBody(JsonObj const& max_body, VirtualHost::Location::s_config& loc_config);
		static void	setLocDirList(JsonObj const& dir_list, VirtualHost::Location::s_config& loc_config);
		static void	setLocAllowedRequest(JsonObj const& allowed_request, VirtualHost::Location::s_config& loc_config);
		static void	setLocCgi(JsonObj const& cgi, VirtualHost::Location::s_config& loc_config);
		static void	setLocCgiExt(JsonObj const& cgi_ext, VirtualHost::Location::s_config& loc_config);
		static void	setLocUploadPath(JsonObj const& upload_path, VirtualHost::Location::s_config& loc_config);

		static void	setHostDefMaxBody(VirtualHost::s_config& host_config);
		static void	setHostDefInterface(VirtualHost::s_config& host_config);
		static void	setHostDefDirList(VirtualHost::s_config& host_config);
		static void	setHostDefAllowedRequest(VirtualHost::s_config& host_config);
		static void	setHostDefCgi(VirtualHost::s_config& host_config);

		static void	setLocDefMaxBody(VirtualHost::Location::s_config& host_config);
		static void	setLocDefDirList(VirtualHost::Location::s_config& loc_config);
		static void	setLocDefCgi(VirtualHost::Location::s_config& loc_config);
};

#endif // VHOSTPARSER_HPP