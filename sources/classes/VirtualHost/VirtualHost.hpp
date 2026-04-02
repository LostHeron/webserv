/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/04/02 17:38:44 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <vector>
#include <map>

class VirtualHost
{
	public:

		VirtualHost(void);
		VirtualHost(VirtualHost const& other);
		~VirtualHost();

		VirtualHost&	operator=(VirtualHost const& other);

	private:

		enum e_reqtype
		{
			GET = 0,
			POST,
			DELETE
		};

		enum e_logtype
		{
			NONE = 0,
			SUCCESS,
			ERROR,
			ALL
		};
		
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
				unsigned int	_allowedReqtypeBitfield;

				enum e_logtype	_logs;
				std::ofstream	_successLogs;
				std::ofstream	_errorLogs;
		};	
	
	
		std::vector<std::string>		_hosts;
		std::string						_root;
		std::string						_index;
		std::map<int, std::string>		_errorPage;
		unsigned int					_allowedReqtypeBitfield;

		enum e_logtype					_logs;
		std::ofstream					_successLogs;
		std::ofstream					_errorLogs;
	
		std::map<std::string, Location>	_locations;
};
