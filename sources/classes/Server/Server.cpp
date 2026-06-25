/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 16:36:39 by jweber            #+#    #+#             */
/*   Updated: 2026/06/11 16:36:48 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ASocket.hpp"
#include "ListenSocket.hpp"
#include "VHostList.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include <algorithm>
#include <stdint.h>
#include <sys/epoll.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Connection/Connection.hpp"

Server::Server(char *config_file):
	isChildren(false),
	status(SUCCESS),
	epoll(),
	host_list(VHostList::build(config_file))
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
			CreateFd(ports[i], 0, *this);
		}
	}
};

Server::~Server()
{
	#ifdef DEBUG
	std::cout << "In SERVER DESTRUCTOR\n";
	#endif
	for (size_t	i = 0; i < this->listenSockets.size(); i++)
	{
		delete (this->listenSockets[i]);
	}
	for (size_t	i = 0; i < this->connections.size(); i++)
	{
		delete (this->connections[i]);
	}
}

void	Server::setIsChildren()
{
	this->isChildren = true;
}

bool	Server::getIsChildren()
{
	return (this->isChildren);
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
void	Server::add(ListenSocket* newListenSocket)
{
	if (this->epoll.add(newListenSocket, EPOLLIN) != SUCCESS)
	{
		std::cerr << "could not add FD to epoll interest list!\n";
	}
	this->listenSockets.push_back(newListenSocket);
}

void	Server::add(ASocket *abstractSocket, int event)
{
	if (this->epoll.add(abstractSocket, event) != SUCCESS)
		std::cerr << "could not add FD to epoll interest list!\n";
}

void	Server::add(Connection* newConnection)
{
	this->connections.push_back(newConnection);
}

void	Server::remove(Connection* toBeDeleted)
{
	std::cout << "REMOVING A CONNECTION !!\n";
	std::vector<Connection *>::iterator it;

	it = std::find(this->connections.begin(), this->connections.end(), toBeDeleted);
	if (it != this->connections.end())
		this->connections.erase(it);

	delete toBeDeleted;
}

int	Server::getEfd()
{
	return (this->epoll.getFd());
}

const VHostList& Server::getHostList() const {return (this->host_list);};

void	Server::remove(ASocket *abstractSocket)
{
	this->epoll.remove(abstractSocket);
}

std::vector<Connection*>&	Server::getConnections()
{
	return (this->connections);
}
