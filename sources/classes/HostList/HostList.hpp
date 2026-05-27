/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostList.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:39:05 by cviel             #+#    #+#             */
/*   Updated: 2026/05/20 14:30:20 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOSTLIST_HPP
# define HOSTLIST_HPP

# include <stdint.h>
# include "VirtualHost.hpp"

class HostList
{
	public:

		HostList(HostList const& other);
		~HostList();

		static HostList	build(char const* filename);

	private:

		std::map<uint16_t, std::vector<VirtualHost> >	_map;

		HostList(void);

		HostList&	operator=(HostList const& other);

		static void	addHost(std::pair<uint16_t, VirtualHost> const& vhost, std::map<uint16_t, std::vector<VirtualHost> >& host_map);
};

#endif