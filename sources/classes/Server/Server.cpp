/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:51:29 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 16:10:49 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ASocket.hpp"
#include "HostList.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include <algorithm>
#include <stdint.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <iostream>
#include <vector>

Server::Server(char *config_file):
	status(SUCCESS),
	epoll(),
	host_list(HostList::build(config_file))
{
	if (this->epoll.fail())
	{
		this->status = FAILURE;
	}
	else
	{
		const std::vector<uint16_t> &ports = this->host_list.getPort();
		for (size_t i = 0; i < ports.size(); i++)
		{
			std::cout << "opening port : " << ports[i] << "\n";
			CreateFd(ports[i], 0, *this);
		}
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

// function used to add the ASocket pointer 
void	Server::add(ASocket* abstract_socket, int event_flags)
{
	if (this->epoll.add(abstract_socket, event_flags) != SUCCESS)
	{
		this->nonBlockingsFds.push_back(abstract_socket);
	}
	else
		this->sockets.push_back(abstract_socket);
}

int	Server::getEfd()
{
	return (this->epoll.getFd());
}

const HostList& Server::getHostList() const {return (this->host_list);};

void	Server::remove(ASocket *asocket)
{
	this->epoll.remove(asocket);
	std::vector<ASocket *>::iterator it;

	it = std::find(this->sockets.begin(), this->sockets.end(), asocket);
	if (it != this->sockets.end())
		this->sockets.erase(it);

	it = std::find(this->nonBlockingsFds.begin(), this->nonBlockingsFds.end(), asocket);
	if (it != this->nonBlockingsFds.end())
		this->nonBlockingsFds.erase(it);

	delete asocket;
}

std::vector<ASocket*>&	Server::getNonBlockingsFds()
{
	return (this->nonBlockingsFds);
}
