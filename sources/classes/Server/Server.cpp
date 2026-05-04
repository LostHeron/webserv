/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:51:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:25:51 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "AFd.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include <algorithm>
#include <sys/epoll.h>
#include <cstdlib>

Server::Server():
	status(SUCCESS),
	epoll()
{
	if (this->epoll.fail())
	{
		this->status = FAILURE;
	}
	else
	{
		CreateFd(0, 4343, *this);
	}
};

Server::~Server()
{
	for (size_t	i = 0; i < this->sockets.size(); i++)
	{
		delete (this->sockets[i]);
	}
}

bool	Server::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}

void	Server::setFailure(int value)
{
	this->status = value;
}

// function used to add the AFd pointer 
void	Server::add(AFd* fd)
{
	this->epoll.add(fd);
	this->sockets.push_back(fd);
}

int	Server::getEfd()
{
	return (this->epoll.getFd());
}

void	Server::remove(AFd *afd)
{
	delete afd;
	this->sockets.erase(
		std::find(this->sockets.begin(), this->sockets.end(), afd)
	);
}
