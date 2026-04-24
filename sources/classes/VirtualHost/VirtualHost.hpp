/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/04/03 18:49:38 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VIRTUALHOST_HPP__
# define __VIRTUALHOST_HPP__

#include <string>
#include <streambuf>
#include <ostream>
#include <vector>
#include <map>

class VirtualHost
{
	public:

		VirtualHost(void);
		VirtualHost(VirtualHost const& other);
		~VirtualHost();

		VirtualHost&	operator=(VirtualHost const& other);

		// void	log(bool success);

	private:

		enum e_reqtype
		{
			GET = 0,
			POST,
			DELETE
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

				Location(void);
				Location(Location const& other);
				~Location();

				Location&	operator=(Location const& other);
			
			private:

				std::string		_root;
				std::string		_index;
				unsigned int	_allowedReqBitfield;

				// std::ostream*	_successLogs;
				// std::ostream*	_errorLogs;
		};
	
		std::vector<std::string>		_hosts;
		std::string						_root;
		std::string						_index;
		unsigned int					_allowedReqBitfield;
		std::map<int, std::string>		_errorPage;

		// StreambufNull					_streambufNull;
		// std::ostream					_streamNull;
		// std::ostream*					_successLogs;
		// std::ostream*					_errorLogs;
	
		std::map<std::string, Location>	_locations;
};

#endif