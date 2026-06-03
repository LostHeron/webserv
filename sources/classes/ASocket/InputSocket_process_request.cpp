/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_request.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:07:29 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 16:54:44 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include "InputSocket.hpp"
#include "RequestFactory.hpp"
#include "ToOutSocket.hpp"
#include "VirtualHost.hpp"
#include "OutputSocket.hpp"
#include <sys/epoll.h>


void	InputSocket::process_request(size_t& pos)
{
	std::string requested_server_name;
	// ach: build arequest (GET/POST/DEL...) from previoulsy fullfilled inputsocket 
	if (this->headers.count("host"))
	{
		if (this->headers["host"].size() > 0)
			requested_server_name = this->headers["host"].at(0);
		else
			requested_server_name = "";
	}
	else
		requested_server_name = "";
	const VirtualHost& vhost = this->server.getHostList().getHost(this->local_port, requested_server_name);
	RequestFactory facto(*this, vhost);//, VirtualHost &vhost;
	ARequest *req = facto.createElement();

	// ach: execute request building response metadata, then Jules will handle the Client transmission
	Response resp = req->execute();

	delete req;
	
	bool iscgi = false;
	iscgi = true;
	if (iscgi == true)
	{
		this->prepareCGI();
		if (resp.getResourceFd() > 0)
			close(resp.getResourceFd());
	}
	else
	{
		OutputSocket* os = static_cast<OutputSocket*>(this->associatedSocket);

		HeadersBuilder	b;
		os->getOutputBuffer() =  b.initialize()
			.buildStatusLine("HTTP/1.1", resp.getStatus())
		 	.buildDate()
		 	.buildCRLF()
		 	.buildBody(resp.getContent())
		 	.build();

		if (resp.getResourceFd() < 0)
			os->getIsLastBuffer() = true;
		else
		{
			ToOutSocket *tos = new ToOutSocket(resp.getResourceFd(), os->getIsLastBuffer(), os->getOutputBuffer(), this->server);
			this->server.add(tos, EPOLLIN);
			this->associatedToOutSocket = tos;
		}
	}
	
	this->state++;
	if (pos < this->input_buffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}
