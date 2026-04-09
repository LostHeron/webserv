/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 10:54:45 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 13:57:56 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include <vector>
#include "Listen.hpp"
#include "EpollStruct.hpp"

class Server
{
	public:
		Server();
		~Server();

		bool	fail();
		void	activate();

	protected:

	private:
		// this structure should not be instantiable from
		// another same structure
		Server(const Server& other);
		Server&	operator=(const Server& other);

		int status;

		// the fd associated with the epoll instance
		// initialized with epoll_create;
		EpollStruct				epoll; 

		// vectors of fds (sockets) associated with all listening ports
		// initialized with socket + bind + listen;
		std::vector<Listen>		listens;
		
		// some kind of structure to remember
		// if an fd is associated with something
		// we should do an 'accept' on or a 'read/write' from/to !
		//std::map<int, AFd*>		fds;
};

#endif
