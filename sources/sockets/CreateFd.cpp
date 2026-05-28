/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CreateFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:14:12 by jweber            #+#    #+#             */
/*   Updated: 2026/05/27 17:01:20 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "ListenSocket.hpp"
#include "InputSocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <netinet/in.h>
#include <stdint.h>
#include <exception>
#include <iostream>

void	CreateFd(uint16_t port, uint32_t addr, Server& server)
{
	try
	{
		ListenSocket* fd = new ListenSocket(port, addr, server);
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

void	CreateFd(int fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server)
{
	try
	{
		InputSocket * new_fd = new InputSocket(fd, local_port, addr, server);
		server.add(new_fd);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		server.setFailure(FAILURE);
	}
}
