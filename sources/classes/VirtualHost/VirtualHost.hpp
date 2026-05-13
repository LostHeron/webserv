/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/05/13 17:39:23 by cviel            ###   ########.fr       */
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

		VirtualHost(JsonObj const& host_obj);
		~VirtualHost();
		
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
			
				Location(JsonObj const& location_obj);
				~Location();
			
			private:
			
				struct s_redir
				{
					std::string	from;
					std::string	to;	
				};
			
				Location(Location const& other);

				Location&	operator=(Location const& other);
				
				std::string					_name;
				std::string					_root;
				std::string					_index;
				std::vector<std::string>	_allowedRequests;
				std::vector<struct s_redir>	_redirections;
			
			
			// std::ostream*	_successLogs;
			// std::ostream*	_errorLogs;
		};
		
		VirtualHost(VirtualHost const& other);

		VirtualHost&	operator=(VirtualHost const& other);

		std::vector<std::string>		_name;
		std::string						_root;
		std::string						_index;
		std::vector<std::string>		_allowedRequests;
		std::map<int, std::string>		_errorPage;
		std::vector<struct s_ip_ramge>	_allowedInterfaces;
		std::map<std::string, Location>	_locations;
		
		// StreambufNull					_streambufNull;
		// std::ostream						_streamNull;
		// std::ostream*					_successLogs;
		// std::ostream*					_errorLogs;
	};
		
		#endif // VIRUTALHOST_HPP