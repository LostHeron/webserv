/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostList.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:39:05 by cviel             #+#    #+#             */
/*   Updated: 2026/05/19 16:02:15 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOSTLIST_HPP
# define HOSTLIST_HPP

# include "VirtualHost.hpp"

class HostList
{
	public:

		HostList(HostList const& other);
		~HostList();

		static HostList	build(char const* filename);

	private:

		std::map<u_int16_t, std::vector<VirtualHost>>	_map;

		HostList(void);

		HostList&	operator=(HostList const& other);

		static void	addHost(std::pair<u_int16_t, VirtualHost> const& vhost, std::map<u_int16_t, std::vector<VirtualHost>>& host_map);
};

#endif