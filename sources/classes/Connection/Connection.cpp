/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:21:57 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 18:48:07 by jweber           ###   ########.fr       */
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
#include <netinet/ip.h>
#include <sys/epoll.h>

Connection::Connection(int fd, uint16_t newLocalPort, const struct sockaddr_in& newPeerAddr, Server& server):
	memoryUsage(0),
	startTime(time(NULL)),
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
	uint32_t addrh = (newPeerAddr.sin_addr.s_addr);
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
		std::cout << "KILLING underlying process\n";
		#endif
		if (kill(this->cgiPid, SIGTERM) < 0)
		{
			int	error_value = errno;
			logerror("kill", error_value);
		}
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


void	Connection::removeFromInterestList(ASocket* abstractSocket)
{
	if (this->server.getIsChildren() == false)
	{
		this->server.remove(abstractSocket);
	}
}

void	Connection::addToInterestList(ASocket* abstractSocket, int flags)
{
	if (this->server.getIsChildren() == false)
	{
		this->server.add(abstractSocket, flags);
	}
}

void	Connection::remove(ASocket* abstractSocket)
{
	if (this->server.getIsChildren() == false && this->cgiPid > 0)
	{
		#ifdef DEBUG
		std::cout << "KILLING Underlying process from remove\n";
		#endif
		if (kill(this->cgiPid, SIGTERM) < 0)
		{
			int	error_value = errno;
			logerror("kill", error_value);
		}
	}
	this->removeFromInterestList(abstractSocket);
}

void			Connection::process()
{
	// get data to process 
	std::string to_process;
	if (this->inputSocket.size() > 0) // check if there is data to process
	{

		to_process = this->inputSocket.front();
		this->inputSocket.pop();
	}

	// proces requestMetaData as long as it's not ready
	if (this->requestMetaData.isReady() == false)
	{
		// we must still process headers and stuff from 
		// incoming data
		requestMetaData.process(to_process);
	}

	if (requestMetaData.isReady() == true && this->resp == NULL)
	{
		// call Achill's part which will create the ressource
		// so we can have kind of this stuff here : 
		/*
	std::string requested_server_name;
	if (this->headers.count("host"))
	{
		if (this->headers["host"].size() > 0)
			requested_server_name = this->headers["host"].at(0);
		else
			requested_server_name = "";
	}
	else
		requested_server_name = "";
	const VirtualHost& vhost = this->connection->getHostList().getHost(this->connection->getLocalPort(), requested_server_name);
	//if (vhost.InterfaceAllowed(this->connection->)
	// here should check if the IP from the vhost is accepted or not
	RequestFactory facto(*this, vhost);//, VirtualHost &vhost;
	ARequest *req = facto.createElement();

	Response resp = req->buildResponse();

	delete req;
	
	if (resp.isCGI() == true)
	{
		if (resp.getResource().first > 0)
		{
			close(resp.getResource().first);
			resp.getResource().first = -1;
		}
		this->launch_cgi(resp);
	}
	else
	{
		OutputSocket* os = this->connection->getOutputSocket();

		HeadersBuilder	b;
		b.initialize();
		if (this->version != "")
		{
			b.buildStatusLine("HTTP/1.1", resp.getStatus())
		 	.buildDate()
			.buildCookies(resp.getCookies())
		 	.buildCRLF();
		}
		b.buildBody(resp.getContent());

		os->setup(resp.getResource().first, b.build());
	}
	
	this->state++;
	if (pos < this->inputBuffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
		*/
		// but of course with some modificatiions
	}

	if (this->resp != NULL)
	{
		if (this->resp->isRedir() == true)
		{
		}
		else if (this->resp->isCGI() == true)
		{
			if (to_process != "")
			{
				this->inCGI.addBuffer(to_process);
			}
			std::string to_process_cgi;
			if (this->outCGI.size() > 0)
			{
				// means there is data to process in the cgi queuqe
				to_process_cgi = this->outCGI.front();
				this->outCGI.pop();
			}
			if (cgiMetaData.isReady() == false)
			{
				cgiMetaData.process(to_process_cgi);
			}
		}
		else
		{
			// read from resourceFd and put the stuff in the queue of buffer
			// to be send to outputSocket
			// clear buffer from request, because not needed here ?
			// except if it is a post ?
		}
	}

	// here we can have a part where we read from the resourceFD
	// and put it in the outputSocket (outputSocket.addBuffer(or what
	// we just read)

	if (to_process.size() > 0)
	{
		// we still have body data to process in this part
	}

	// but we have to be careful if it is a cgi, or if it is
	// a redir, we might have some stuff to do is it here that
	// we treat it ?
	
	if (isCgi() == true)
	{
		// then we have some stuff to do too ?
		// we should send some data to be sent from the body
		// so the to_process buffer
		// to the inputCGI
		// and we should look if we need to process the cgiMetaData
		// that are also headers,
		// but should it be in a king of CGI class ?
		// aaarrrrrrrrffff c'est relou ça serait cool de faire ça
		// mais la ça va prendre du temps et c'est relou
		// OMG les travaux, ça tabasse la code base fort la :):)
	}
	
	// aaaaaah ya un truc qui ne me plait pas la, c'est pas beau
	// c'est moche arrgvsdlkmqfjqmsldjfq
	// but we also need to see
	// if (some check)
	// then we must add data to
	
	// some king of flag saying when the processing of the 
	// requestMetaData if over
	// when is 
}

void			Connection::setIsChildren()
{
	this->server.setIsChildren();
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
