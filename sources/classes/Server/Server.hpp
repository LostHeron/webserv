/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:54:45 by jweber            #+#    #+#             */
/*   Updated: 2026/05/27 17:05:54 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include "EpollStruct.hpp"
#include "HostList.hpp"

class ASocket;

class Server
{
	public:
		Server(char *config_file);
		virtual ~Server();

		bool			fail();
		void			add(ASocket*);
		void			remove(ASocket*);
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
		std::vector<ASocket*>		sockets;


		HostList				host_list;
		// some kind of structure to remember
		// if an fd is associated with something
		// we should do an 'accept' on or a 'read/write' from/to !
		//std::map<int, ASocket*>		fds;
};

#endif
