/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/05/18 20:41:30 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUALHOST_HPP
# define VIRTUALHOST_HPP

# include <string>
# include <streambuf>
# include <ostream>
# include <vector>
# include <map>
# include "JsonObj.hpp"

class VirtualHost
{
	public:

		VirtualHost(VirtualHost const& other);
		~VirtualHost();
		
		static std::pair<u_int16_t, VirtualHost>	build(std::map<std::string, JsonObj> obj_map);
		
		// void	log(bool success);
		
	private:

		struct s_ip_range
		{
			u_int32_t	min;	
			u_int32_t	max;	
		};

		struct s_redir
		{
			std::string	from;
			std::string	to;	
		};
		
		// class StreambufNull :
		// 	public std::streambuf
		// {
		// 	public:
		
		// 		StreambufNull(void);
		// 		StreambufNull(StreambufNull const& other);
		// 		~StreambufNull();
		
		// 		StreambufNull&	operator=(StreambufNull const& other);
		
		// 	protected:
		
		// 		virtual int_type		overflow(int ch);
		// 		virtual std::streamsize	xsputn(const char_type* s, std::streamsize count);	
		// 		virtual int				sync(void);		
		// };

		class Location
		{
			public:
			
				Location(Location const& other);
				~Location();

				static std::pair<std::string, Location>	build(std::map<std::string, JsonObj> const& loc_obj);
			
			private:
			
				struct s_redir
				{
					std::string	from;
					std::string	to;	
				};
				
				std::string							_root;
				std::string							_index;
				std::vector<std::string>			_allowedRequest;
				bool								_allowDirList;
				std::vector<Location::s_redir>		_redirection;
				std::map<std::string, std::string>	_cgi;
				
				Location(void);
				
				Location&	operator=(Location const& other);

				static Location::s_redir	buildRedir(std::string const& redir);
				
			// std::ostream*	_successLogs;
			// std::ostream*	_errorLogs;
		};
		
		std::vector<std::string>						_names;
		std::string										_root;
		std::string										_index;
		u_int32_t										_max_body_size;
		std::vector<VirtualHost::s_ip_range>			_allowedInterface;
		std::vector<std::string>						_allowedRequest;
		bool											_allowDirList;
		std::map<int, std::string>						_error;
		std::map<std::string, VirtualHost::Location>	_location;
		std::map<std::string, std::string>				_cgi;
		
		VirtualHost(void);

		VirtualHost&	operator=(VirtualHost const& other);

		template <typename T>
		static bool	checkDuplicates(T const& val, std::vector<T> const& vec);

		static VirtualHost::s_ip_range	buildInterfaceRange(std::string const& interfaces);
		static u_int32_t				buildInterface(std::string const& interface);
		static void						addErrorPage(std::map<std::string, JsonObj> const& error, std::map<int, std::string>& host_error);
		static void						addLocation(std::map<std::string, JsonObj> const& location, std::map<std::string, VirtualHost::Location>& host_location);
		static void						addCgi(std::map<std::string, JsonObj> const& cgi, std::map<std::string, std::string>& host_cgi);		
		
		// StreambufNull					_streambufNull;
		// std::ostream						_streamNull;
		// std::ostream*					_successLogs;
		// std::ostream*					_errorLogs;
	};
		
#endif // VIRUTALHOST_HPP