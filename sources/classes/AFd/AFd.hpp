/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:21:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:37:35 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFD_HPP
# define AFD_HPP

#include "Server.hpp"
#include <cstddef>

class AFd
{
	public:
		AFd(Server& server);
		virtual ~AFd();

		int	getFd();

		virtual void	process() = 0;
		virtual	bool	fail();

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

		// a reference to the server, used to create new instances
		// of IOFd (which inherits from AFd), when ListenFDs processes
		// incomming connections (with the process function)
		Server&	server;

	private:
		AFd(const AFd& other);
		AFd& operator=(const AFd& other);
};

#endif
