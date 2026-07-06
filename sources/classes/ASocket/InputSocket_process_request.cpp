/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_request.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:07:29 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 14:24:07 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include "InputSocket.hpp"
#include "RequestFactory.hpp"
#include "Response.hpp"
#include "VirtualHost.hpp"
#include "OutputSocket.hpp"
#include "Connection.hpp"
#include <stdint.h>
#include <sys/epoll.h>


void	InputSocket::process_request(size_t& pos)
{

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

	
	this->resp = new Response(req->buildResponse());

	delete req;
	
	if (this->resp->isCGI() == true)
	{
		if (this->resp->getResource().first > 0)
		{
			close(this->resp->getResource().first);
			this->resp->getResource().first = -1;
		}
		// we can not launch directly in case of chunked 
		// request, we need the size of the total request before

		char *end;
		size_t body_size;
		if (this->headers.count("content-length") == 1)
			body_size = std::strtol(this->headers["content-length"].at(0).c_str(), &end, 10);
		else
			body_size = 0;
		if (this->connection->isChunked() == false)
			this->launch_cgi(body_size);
	}
	else
	{
		OutputSocket* os = this->connection->getOutputSocket();

		HeadersBuilder	b;
		b.initialize();
		if (this->version != "")
		{
			b.buildStatusLine("HTTP/1.1", resp->getStatus())
		 	.buildDate()
		 	.buildCRLF();
		}
		b.buildBody(resp->getContent());

		os->setup(resp->getResource().first, b.build());
	}
	
	this->state++;
	if (pos < this->inputBuffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}
