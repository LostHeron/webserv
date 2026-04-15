/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CreateFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:14:12 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:36:46 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "ListenFd.hpp"
#include "IOFd.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <stdint.h>
#include <exception>
#include <iostream>

void	CreateFd(uint16_t port, uint32_t addr, Server& server)
{
	try
	{
		ListenFd* fd = new ListenFd(port, addr, server);
		fd->activate();
		if (fd->fail())
		{
			server.setFailure(FAILURE);
			delete fd;
		}
		else
		{
			server.add(fd);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		server.setFailure(FAILURE);
	}
}

void	CreateFd(int fd, Server& server)
{
	try
	{
		IOFd * new_fd = new IOFd(fd, server);
		server.add(new_fd);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		server.setFailure(FAILURE);
	}
}
