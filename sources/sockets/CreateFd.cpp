/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CreateFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:14:12 by jweber            #+#    #+#             */
/*   Updated: 2026/04/10 17:31:50 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AFd.hpp"
#include "ListenFd.hpp"
#include "IOFd.hpp"
#include "Server.hpp"
#include <stdint.h>
#include <exception>
#include <iostream>

#define LISTEN_FD 1
#define IO_FD 2

AFd	*CreateFd(int fd, uint16_t port, uint32_t addr, int type, Server& server)
{
	try
	{
		if (type == LISTEN_FD)
		{
			ListenFd* fd = new ListenFd(port, addr, server);
			fd->activate();
			if (fd->fail())
			{
				delete fd;
				return (NULL);
			}
			return (fd);
		}// also ADD server here
			// or juste wait to pick up the newly created fd and add it to both 
			// interest list and vector of AFd*
			// yep lets go more on that
		else if (type == IO_FD)
			return (new IOFd(fd, server));
		return (NULL);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return (NULL);
	}
}
