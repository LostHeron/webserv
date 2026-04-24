/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:24:31 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 13:42:06 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenFd.hpp"
#include "AFd.hpp"
#include "Server.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <cerrno>

ListenFd::ListenFd(uint32_t address, uint16_t port, Server& server):
	AFd(server)
{
	this->status = SUCCESS;

	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd < 0)
	{
		// socket creation !
		std::string error_msg(strerror(errno));
		std::cerr << "socket: " << error_msg << "\n";
		this->status = FAILURE;
	}

	std::memset(&this->addr_data, 0, sizeof(addr_data));
	this->addr_data.sin_family = AF_INET;
	this->addr_data.sin_addr.s_addr = htonl(address);
	this->addr_data.sin_port = htons(port);
}

ListenFd::~ListenFd()
{
}

void	ListenFd::process()
{
	// ok and here should do stuff with the fd,
	// and read data and start parsing request
	int						peer_fd;
	struct sockaddr_storage peer_addr;
	socklen_t				peer_addr_len;

	peer_fd = accept(this->fd, (struct sockaddr*)&peer_addr, &peer_addr_len);
	if (peer_fd < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "epoll_wait: " << error_msg << "\n";
	}
	else
	{
		std::cout << "a connection was accepted\n";
		// and here should also do stuff like CreateFD
		// and that's here i need my server !!
		// to add the newly created IOFd
		// but not smart to add it to the process argument
		// because process
		// is part of abstract class
		// and and in the IOFd process, no need for a 'Server'
		// args in the process function
	}
	CreateFd(peer_fd, this->server);
	if (this->server.fail())
	{
		std::cout << "an error happened while processing new incoming connection\n";
	}
}

void	ListenFd::activate()
{
	int	ret;

	ret = bind(this->fd, (struct sockaddr *)&this->addr_data, sizeof(this->addr_data));
	if (ret < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "bind: " << error_msg << "\n";
		this->status = FAILURE;
		return ;
	}

	ret = listen(this->fd, 50);
	if (ret < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "listen: " << error_msg << "\n";
		this->status = FAILURE;
		return ;
	}
}


bool	ListenFd::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}
