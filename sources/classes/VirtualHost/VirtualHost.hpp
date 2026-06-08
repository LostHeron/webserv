/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/06/08 19:22:31 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_HPP
# define VIRTUALHOST_HPP

# include <stdint.h>
# include <string>
# include <streambuf>
# include <ostream>
# include <vector>
# include <map>
# include "JsonObj.hpp"

class VirtualHost
{
	public:

		struct s_ip_range
		{
			uint32_t	min;	
			uint32_t	max;
			
			bool	operator==(struct s_ip_range const& other) const {return (this->min == other.min && this->max == other.max);}
		};

		class Location
		{
			public:
			
				struct s_config
				{
					std::string					alias;
					std::string					redirection;
					std::string					index;
					std::vector<std::string>	allowedRequest;
					bool						allowDirList;
					bool						cgi;
					std::vector<std::string>	cgi_ext;
				};
			
				Location(Location const& other);
				~Location();

				static std::pair<std::string, Location>	build(std::map<std::string, JsonObj> const& loc_obj, std::vector<std::string> const& host_allowed_request);
			
			private:
			
				Location::s_config	_conf;
				
				Location&	operator=(Location const& other);
		};

		struct s_config
		{
			std::vector<std::string>						name;
			std::string										root;
			std::string										index;
			uint32_t										max_body_size;
			std::vector<VirtualHost::s_ip_range>			allowedInterface;
			std::vector<std::string>						allowedRequest;
			bool											allowDirList;
			std::map<int, std::string>						error;
			std::map<std::string, VirtualHost::Location>	location;
			bool											cgi;
			std::vector<std::string>						cgi_ext;

		};
		
		struct s_uriInfo
		{
			std::string	path;
				
		};

		VirtualHost(VirtualHost::s_config const& conf);
		VirtualHost(VirtualHost const& other);
		~VirtualHost();

		std::vector<std::string> const&	getName(void) const;
		std::pair<std::string, bool>	getPathReq(std::string const& uri, std::string const& req) const;
		std::pair<bool, std::string> 	getError(int err_code) const;
		
		// void	log(bool success);
		
	private:
		
		struct s_config	_conf;
		
		VirtualHost(void);

		VirtualHost&	operator=(VirtualHost const& other);
};
		
#endif // VIRTUALHOST_HPP