/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:47:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 13:47:52 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"
#include "status.hpp"
#include <netinet/ip.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <iostream>
#include <unistd.h>

// maybe it does make sense to use exception 
// in the part of this constructor !
Listen::Listen(uint32_t	address, uint16_t port)
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

Listen::Listen(const Listen& other):
	status(other.status),
	addr_data(other.addr_data)
{
	fd = dup(other.fd);
	if (fd < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "dup: " << error_msg << "\n";
		this->status = FAILURE;
	}
}

Listen::~Listen()
{
	close(this->fd);
}

void	Listen::activate()
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


bool	Listen::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}
