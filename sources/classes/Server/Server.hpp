/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 16:38:25 by jweber            #+#    #+#             */
/*   Updated: 2026/06/11 16:38:26 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include "EpollStruct.hpp"
#include "VHostList.hpp"

class Connection;
class ListenSocket;

class Server
{
	public:
		Server(char *config_file);
		virtual ~Server();

		bool			fail();

		void			setIsChildren();
		bool			getIsChildren();

		void			add(ListenSocket*);

		void			add(ASocket*, int event);
		void			remove(ASocket*);

		void			add(Connection*);
		void			remove(Connection*);

		void			setFailure(int value);
		int				getEfd();
		const VHostList&	getHostList() const;

		std::vector<Connection*>&	getConnections();

	protected:

	private:
		// this structure should not be instantiable from
		// another same structure
		Server();
		Server(const Server& other);
		Server&	operator=(const Server& other);

		bool	isChildren;

		int		status;

		// the fd associated with the epoll instance
		// initialized with epoll_create;
		EpollStruct				epoll; 

		// vectors of fds (sockets) associated with all listening ports
		// initialized with socket + bind + listen;
		std::vector<Connection*>		connections;
		std::vector<ListenSocket*>		listenSockets;


		VHostList				host_list;
		// some kind of structure to remember
		// if an fd is associated with something
		// we should do an 'accept' on or a 'read/write' from/to !
		//std::map<int, ASocket*>		fds;
};

#endif
