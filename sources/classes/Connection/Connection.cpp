/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:21:57 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:29:42 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ASocket.hpp"
#include <ctime>
#include <netinet/ip.h>
#include <sys/epoll.h>

Connection::Connection(int fd, uint16_t newLocalPort, const struct sockaddr_in& newPeerAddr, Server& server):
	startTime(time(NULL)),
	peerPort(ntohs(newPeerAddr.sin_port)),
	localPort(newLocalPort),
	inputSocket(fd, this),
	outputSocket(fd, this),
	inCGI(NULL),
	outCGI(NULL),
	server(server)
{
	uint32_t addrh = (newPeerAddr.sin_addr.s_addr);
	for (int i = 0; i < 4; i++)
	{
		this->peerAddr[i] = ( reinterpret_cast<uint8_t *>(&addrh) )[i];
	}

	this->server.add(&inputSocket, EPOLLIN);
	this->server.add(&outputSocket, EPOLLOUT);
}

Connection::~Connection()
{
	this->server.remove(&inputSocket);
	this->server.remove(&outputSocket);

	if (this->inCGI != NULL)
		this->server.remove(this->inCGI);
	delete this->inCGI;
	this->inCGI = NULL;

	if (this->outCGI != NULL)
		this->server.remove(this->outCGI);
	delete this->outCGI;
	this->outCGI = NULL;
}

void	Connection::add(ASocket* abstractSocket, int event)
{
	this->server.add(abstractSocket, event);
}

void	Connection::remove(ASocket* abstractSocket)
{
	this->server.remove(abstractSocket);
}

InputSocket*	Connection::getInputSocket() {return (&this->inputSocket);}
OutputSocket*	Connection::getOutputSocket() {return (&this->outputSocket);}
InCGI*			Connection::getInCGI() {return (this->inCGI);}
OutCGI*			Connection::getOutCGI(){return(this->outCGI);}

uint8_t			*Connection::getPeerAddr() {return (this->peerAddr);}
uint16_t		Connection::getPeerPort() {return (this->peerPort);}
uint16_t		Connection::getLocalPort() {return (this->localPort);}

const HostList& Connection::getHostList() const {return (this->server.getHostList());}

time_t			Connection::getStartTime() const {return (this->startTime);}
