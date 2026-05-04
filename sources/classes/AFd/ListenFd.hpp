/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenFd.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:24:35 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:37:01 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENFD_HPP
# define LISTENFD_HPP

#include "AFd.hpp"
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

class ListenFd: public AFd
{
	public:
		ListenFd(uint32_t address, uint16_t port, Server& server);
		~ListenFd();

		void	process();
		void	activate();

	protected:

	private:
		ListenFd();
		ListenFd(const ListenFd& other);
		ListenFd& operator=(const ListenFd& other);

		struct sockaddr_in	addr_data;
};

#endif
