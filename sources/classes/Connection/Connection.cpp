/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:21:57 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 14:26:46 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Connection.hpp"
#include "ASocket.hpp"
#include "typedef.hpp"
#include "error.hpp"
#include <cerrno>
#include <csignal>
#include <ctime>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>

Connection::Connection(int fd, uint16_t newLocalPort, const struct sockaddr_in& newPeerAddr, Server& server):
	memoryUsage(0),
	startTime(time(NULL)),
	lastReceivedTime(startTime),
	chunked(false),
	vHost(NULL),
	peerPort(ntohs(newPeerAddr.sin_port)),
	localPort(newLocalPort),
	inputSocket(fd, this),
	outputSocket(fd, this),
	cgiPid(-1),
	inCGI(NULL),
	outCGI(NULL),
	server(server)
{
	addrh = ntohl(newPeerAddr.sin_addr.s_addr);
	for (int i = 0; i < 4; i++)
	{
		this->peerAddr[i] = ( reinterpret_cast<uint8_t *>(&addrh) )[i];
	}

	this->server.add(&this->inputSocket, EPOLLIN);
	this->server.add(&this->outputSocket, EPOLLOUT);
}

Connection::~Connection()
{
	#ifdef DEBUG
		std::cout << "IN CONNECTION DESTRUCTOR concerning uri: '" << this->getInputSocket()->getUri() << "'\n";
	#endif
	if (this->getIsChildren() == false)
	{
		this->server.remove(&inputSocket);
		this->server.remove(&outputSocket);
	}

	if (this->getIsChildren() == false && this->cgiPid > 0)
	{
		#ifdef DEBUG
		std::cout << "KILLING Underlying process from remove\n";
		#endif

		int ret = kill(this->cgiPid, SIGTERM);
		(void) ret;

		#ifdef DEBUG
		if (kill(this->cgiPid, SIGTERM) < 0)
		{
			int	error_value = errno;
			logerror("kill", error_value);
		}
		#endif
		
		this->cgiPid = -1;
	}
	if (this->inCGI != NULL)
	{
		//this->server.remove(this->inCGI);
		//
		if (this->getIsChildren() == false)
			this->server.remove(this->inCGI);
		delete this->inCGI;
		this->inCGI = NULL;
	}

	if (this->outCGI != NULL)
	{
		if (this->getIsChildren() == false)
			this->server.remove(this->outCGI);
		delete this->outCGI;
		this->outCGI = NULL;
	}
}

void	Connection::add(ASocket* abstractSocket, int event)
{
	this->server.add(abstractSocket, event);
}

void	Connection::remove(ASocket* abstractSocket)
{
	if (this->server.getIsChildren() == false && this->cgiPid > 0)
	{
		#ifdef DEBUG
		std::cout << "KILLING Underlying process from remove\n";
		#endif

		int ret = kill(this->cgiPid, SIGTERM);
		(void) ret;

		#ifdef DEBUG
		if (kill(this->cgiPid, SIGTERM) < 0)
		{
			int	error_value = errno;
			logerror("kill", error_value);
		}
		#endif
		
		this->cgiPid = -1;
	}
	if (this->server.getIsChildren() == false)
	{
		this->server.remove(abstractSocket);
	}
}

void			Connection::setChunked()
{
	this->chunked = true;
}

bool			Connection::isChunked()
{
	return (this->chunked);
}

Chunk&			Connection::getChunk()
{
	return (this->chunk);
}

bool			Connection::isCGI()
{
	if (this->getCgiPid() >= 0)
		return (true);
	else
		return (false);
}

void			Connection::addMemoryUsage(size_t size)
{
	this->memoryUsage += size;
}

size_t			Connection::getMemoryUsage()
{
	return (this->memoryUsage);
}


void			Connection::setIsChildren()
{
	this->server.setIsChildren();
}

uint32_t		Connection::getAddrh()
{
	return (this->addrh);
}

void		Connection::printAddrh()
{
	std::cout 
		<< (int)peerAddr[0] << "."
		<< (int)peerAddr[1] << "."
		<< (int)peerAddr[2] << "."
		<< (int)peerAddr[3] << "\n"
	;
}

bool			Connection::getIsChildren()
{
	return (this->server.getIsChildren());
}

InputSocket*	Connection::getInputSocket() {return (&this->inputSocket);}
OutputSocket*	Connection::getOutputSocket() {return (&this->outputSocket);}

InCGI*			Connection::getInCGI() {return (this->inCGI);}
void			Connection::setInCGI(InCGI* icgi) {this->inCGI = icgi;}

OutCGI*			Connection::getOutCGI(){return(this->outCGI);}
void			Connection::setOutCGI(OutCGI* ocgi) {this->outCGI = ocgi;}

int				Connection::getCgiPid() {return (this->cgiPid);}
void			Connection::setCgiPid(int pid) {this->cgiPid = pid;}

uint8_t			*Connection::getPeerAddr() {return (this->peerAddr);}
uint16_t		Connection::getPeerPort() {return (this->peerPort);}
uint16_t		Connection::getLocalPort() {return (this->localPort);}

const VHostList& Connection::getHostList() const {return (this->server.getHostList());}

time_t			Connection::getStartTime() const {return (this->startTime);}
time_t			Connection::getLastRecievedTime() const {return (this->lastReceivedTime);}
void			Connection::updateLastReceivedTime()
{
	this->lastReceivedTime = time(NULL);
}

const VirtualHost		*Connection::getVHost()
{
	return (this->vHost);
}

void			Connection::setVHost()
{
	std::string	requested_host_name;
	string_map&	headers = this->getInputSocket()->getHeadersNoConst();
	if (headers.count("host") == 1 && headers["host"].size() > 0)
		requested_host_name = headers["host"].at(0);
	else
		requested_host_name = "";

	this->vHost = &this->getHostList().getHost(this->localPort, requested_host_name);
	return;
}
