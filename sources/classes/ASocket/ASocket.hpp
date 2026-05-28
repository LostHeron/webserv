/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:21:29 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:49:08 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET_HPP
# define ASOCKET_HPP

#include "Server.hpp"
#include <cstddef>

class ASocket
{
	public:
		ASocket(Server& server);
		virtual ~ASocket();

		int	getFd() const;

		virtual void	process() = 0;
		virtual	bool	fail();
		
		void	setAssociatedSocket(ASocket *ptr);

	protected:

		// the fd corresponding to the connection
		// it will be used by subsequent call 
		// to read, write, recv, sendto
		int		fd;

		// used to keep track of memory usage by each
		// request, in order to chose which connection 
		// to abort first;
		size_t	memoryUsage;

		// used to indicate wether an error occured
		// 0 = no probleme (0 should be same as SUCCESS)
		// non nul indicating a problem
		int		status;

		ASocket	*associatedSocket;
		// a reference to the server, used to create new instances
		// of InputSocket (which inherits from ASocket), when ListenSockets processes
		// incomming connections (with the process function)
		Server&	server;

	private:
		ASocket(const ASocket& other);
		ASocket& operator=(const ASocket& other);
};

#endif
