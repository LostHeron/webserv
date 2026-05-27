/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:24:35 by jweber            #+#    #+#             */
/*   Updated: 2026/05/27 17:07:47 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENSOCKET_HPP
# define LISTENSOCKET_HPP

#include "ASocket.hpp"
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

class ListenSocket: public ASocket
{
	public:
		ListenSocket(uint16_t port, uint32_t address, Server& server);
		~ListenSocket();

		void	process();
		void	activate();

	protected:

	private:
		ListenSocket();
		ListenSocket(const ListenSocket& other);
		ListenSocket& operator=(const ListenSocket& other);

		struct sockaddr_in	addr_data;
};

#endif
