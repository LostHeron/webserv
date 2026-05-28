/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CreateFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:14:12 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:50:04 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "ListenSocket.hpp"
#include "InputSocket.hpp"
#include "OutputSocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <netinet/in.h>
#include <stdint.h>
#include <exception>
#include <iostream>
#include <sys/epoll.h>

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
			server.add(fd, EPOLLIN);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		server.setFailure(FAILURE);
	}
}

void	CreateFd(int socket_fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server)
{
	try
	{
		OutputSocket * new_output_socket = new OutputSocket(socket_fd, server);
		server.add(new_output_socket, EPOLLOUT);
		std::cout << "added ouputsocket\n";
		InputSocket * new_input_socket = new InputSocket(socket_fd, local_port, addr, server);
		server.add(new_input_socket, EPOLLIN);
		std::cout << "added inputsocket\n";
		new_output_socket->setAssociatedSocket(new_input_socket);
		new_input_socket->setAssociatedSocket(new_output_socket);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		server.setFailure(FAILURE);
	}
}
