/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:01:33 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 18:14:00 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <unistd.h>

ASocket::ASocket(Server& server):
	fd(-1),
	memoryUsage(0),
	status(SUCCESS),
	associatedSocket(NULL),	
	server(server)
{
}

ASocket::~ASocket()
{
	close(this->fd);
	if (this->associatedSocket != NULL)
	{
		this->associatedSocket->associatedSocket = NULL;
		server.remove(this->associatedSocket);
	}
}

int	ASocket::getFd() const
{
	return (this->fd);
}

bool	ASocket::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}

void	ASocket::setAssociatedSocket(ASocket *ptr)
{
	this->associatedSocket = ptr;
}
