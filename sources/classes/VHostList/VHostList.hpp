/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHostList.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:14:01 by cviel             #+#    #+#             */
/*   Updated: 2026/06/10 19:26:11 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VHOSTLIST_HPP
# define VHOSTLIST_HPP

# include <stdint.h>
# include "VirtualHost.hpp"

class VHostList
{
	public:

		typedef std::map<uint16_t, std::vector<VirtualHost> > HostMap;	
	
		VHostList(VHostList const& other);
		~VHostList();

		static VHostList	build(char const* filename);

		std::vector<uint16_t>	getPort(void) const;
		VirtualHost const&		getHost(uint16_t port, std::string const& name) const;

	private:

		HostMap	_hosts;

		VHostList(void);

		VHostList&	operator=(VHostList const& other);

		static void	addHost(std::pair<uint16_t, VirtualHost> const& vhost, HostMap& host_map);
};

#endif // VHOSTLIST_HPP