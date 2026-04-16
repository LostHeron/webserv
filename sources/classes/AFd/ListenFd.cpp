/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenFd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:24:31 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:36:20 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenFd.hpp"
#include "AFd.hpp"
#include "Server.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include <asm-generic/socket.h>
#include <cstring>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <cerrno>
#include <fcntl.h>

ListenFd::ListenFd(uint32_t address, uint16_t port, Server& server):
	AFd(server)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);

	// to allow reusing the same port, should be disabled in prod ?
	int	optval = 1;
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0)
	{
		std::cerr << strerror(errno) << "\n";
		std::cerr << "error occured will trying to set SO_REUSEPORT!\n";
	}

	if (this->fd < 0)
	{
		// socket creation !
		std::string error_msg(strerror(errno));
		std::cerr << "socket: " << error_msg << "\n";
		this->status = FAILURE;
	}
	else if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "fcntl: " << error_msg << "\n";
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
	struct sockaddr_in		peer_addr;
	socklen_t				peer_addr_len;

	do 
	{
		std::memset(&peer_addr, 0, sizeof(peer_addr));
		peer_addr_len = sizeof(peer_addr);
		peer_fd = accept(this->fd, (struct sockaddr*)&peer_addr, &peer_addr_len);
		if (peer_addr_len > sizeof(peer_addr))
			std::cerr << "WARNING ! peer_addr_len > sizeof(peer_addr), "
					"BUT THIS IS THE CASE HERE, SOMETHING WENT WRONG"
					"AND WAS NOT EXPECTED!\n";
		if (peer_fd < 0)
			break;
		std::cout << "a connection was accepted\n";

		CreateFd(peer_fd, peer_addr, this->server);

		if (this->server.fail())
		{
			std::cout << "an error happened while processing new incoming connection\n";
			return ;
		}
	}
	while (peer_fd >= 0);
	return ;
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
