/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:01:33 by jweber            #+#    #+#             */
/*   Updated: 2026/05/27 17:05:22 by jweber           ###   ########.fr       */
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
	server(server)
{
}

ASocket::~ASocket()
{
	close(this->fd);
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
