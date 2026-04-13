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
	close(this->fd);
}

void	ListenFd::process()
{
	struct sockaddr_in	peer_addr;
	socklen_t			peer_addr_len;
	int	peer_fd = accept(this->fd, (struct sockaddr*)&peer_addr, &peer_addr_len);
	if (peer_fd < 0)
	{
		this->status = FAILURE;
		return;
	}
	// and here should call the factory to create a new AFd* that should be put inside 
	// the vector of AFd* in the server class,
	// but also add it to the epoll instance !
	// hm so does my epoll instance should be in the AFd* ?
	// nope but there's stuff to think about !
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
