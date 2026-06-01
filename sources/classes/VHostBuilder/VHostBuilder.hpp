/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostBuilder.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 15:56:58 by cviel             #+#    #+#             */
/*   Updated: 2026/06/01 19:42:42 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VHOSTBUILDER_HPP
# define VHOSTBUILDER_HPP

# include <stdint.h>
# include <string>
# include <map>
# include "VirtualHost.hpp"
# include "JsonObj.hpp"

class VHostBuilder
{
	public:

		static std::pair<uint16_t, VirtualHost>	jsonBuild(std::map<std::string, JsonObj> const& obj_map);

	private:

		static const	std::map<std::string, std::pair<void (*)(JsonObj const&), void (*)(void)> >	_processMap;
	
		template <typename T>
		static bool	checkDuplicates(T const& val, std::vector<T> const& vec);
		
		static VirtualHost::s_ip_range	buildInterfaceRange(std::string const& interfaces);
		static uint32_t					buildInterface(std::string const& interface);
		static void						addErrorPage(std::map<std::string, JsonObj> const& error, std::map<int, std::string>& host_error);
		static void						addLocation(std::map<std::string, JsonObj> const& location, std::map<std::string, VirtualHost::Location>& host_location, std::vector<std::string> const& host_allowed_request);
		static void						addCgi(std::map<std::string, JsonObj> const& cgi, std::map<std::string, std::string>& host_cgi);

		static void	setName(JsonObj const& names);
		static void	setRoot(JsonObj const& root);
		static void	setIndex(JsonObj const& index);
		static void	setMaxBody(JsonObj const& max_body);
		static void	setInterface(JsonObj const& interface);
		static void	setDirList(JsonObj const& dir_list);
		static void	setAllowedRequest(JsonObj const& allowed_request);
		static void	setError(JsonObj const& error);
		static void	setLocation(JsonObj const& location);
		static void	setCgi(JsonObj const& cgi);

};

#endif // VHOSTBUILDER_HPP