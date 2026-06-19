/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/06/19 15:49:35 by cviel            ###   ########.fr       */
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
			
				Location::s_config const	conf;

				Location(Location::s_config const& conf);
				Location(Location const& other);
				~Location();
			
			private:
			
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
		
		class UriInfo
		{
			public:

				friend class VirtualHost;
			
				UriInfo(VirtualHost::s_config conf);
				UriInfo(UriInfo const& other);

				bool				isRedir(void)	const;
				std::string const&	getRealPath(void) const;
				std::string const&	getIndex(void) const;
				bool				isRequestAllowed(std::string const& req) const;
				bool				isDirListAllowed(void) const;
				bool				isCgiAllowed(void) const;
				bool				isCgiExtAllowed(std::string const& cgi_ext) const;
				
			private:
				UriInfo();
				const UriInfo& operator=(const UriInfo& other);
				bool						_isRedir;
				std::string					_path;
				std::string					_index;
				std::vector<std::string>	_allowedRequests;
				bool						_allowDirList;
				bool						_cgi;
				std::vector<std::string>	_cgi_ext;
		};

		VirtualHost(VirtualHost::s_config const& conf);
		VirtualHost(VirtualHost const& other);
		~VirtualHost();

		std::vector<std::string> const&	getName(void) const;
		bool							InterfaceAllowed(uint32_t interface) const;
		uint64_t						getBodySize(void) const;
		std::pair<bool, std::string> 	getError(int err_code) const;
		UriInfo							getUriInfo(std::string const& uri) const;
		
	private:
		
		struct s_config	_conf;
		
		VirtualHost(void);

		VirtualHost&	operator=(VirtualHost const& other);

		static void		buildUriInfo(std::string const& uri, std::pair<std::string, Location> const& loc_pair, UriInfo& uri_info);
};
		
#endif // VIRTUALHOST_HPP
