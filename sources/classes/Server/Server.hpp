/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:54:45 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 15:03:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include "EpollStruct.hpp"
#include "HostList.hpp"

class Connection;
class ListenSocket;

class Server
{
	public:
		Server(char *config_file);
		virtual ~Server();

		bool			fail();

		void			add(ListenSocket*);

		void			add(ASocket*, int event);
		void			remove(ASocket*);

		void			add(Connection*);
		void			remove(Connection*);

		void			setFailure(int value);
		int				getEfd();
		const HostList& getHostList() const;

	protected:

	private:
		// this structure should not be instantiable from
		// another same structure
		Server();
		Server(const Server& other);
		Server&	operator=(const Server& other);

		int status;

		// the fd associated with the epoll instance
		// initialized with epoll_create;
		EpollStruct				epoll; 

		// vectors of fds (sockets) associated with all listening ports
		// initialized with socket + bind + listen;
		std::vector<Connection*>		connections;
		std::vector<ListenSocket*>		listenSockets;


		HostList				host_list;
		// some kind of structure to remember
		// if an fd is associated with something
		// we should do an 'accept' on or a 'read/write' from/to !
		//std::map<int, ASocket*>		fds;
};

#endif
