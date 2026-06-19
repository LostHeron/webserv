/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_request.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:07:29 by jweber            #+#    #+#             */
/*   Updated: 2026/06/19 16:33:29 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include "InputSocket.hpp"
#include "RequestFactory.hpp"
#include "Response.hpp"
#include "VirtualHost.hpp"
#include "OutputSocket.hpp"
#include <stdint.h>
#include <sys/epoll.h>
#include "Connection/Connection.hpp"


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
	const VirtualHost& vhost = this->connection->getHostList().getHost(this->connection->getLocalPort(), requested_server_name);
	RequestFactory facto(*this, vhost);//, VirtualHost &vhost;
	ARequest *req = facto.createElement();

	// ach: execute request building response metadata, then Jules will handle the Client transmission
	Response resp = req->execute();

	vhost.getUriInfo(this->uri);
	//VirtualHost::UriInfo a();
	//std::cout << "Real Path = " << a.getRealPath() << "\n";
	delete req;
	
	bool iscgi = false;
	//iscgi = true;
	if (iscgi == true)
	{
		if (resp.getResource().first > 0)
			close(resp.getResource().first);
		this->prepareCGI(resp.getResource().second);
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
		 	.buildCRLF();
		}
		b.buildBody(resp.getContent());

		os->setup(resp.getResource().first, b.build());
	}
	
	this->state++;
	if (pos < this->inputBuffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}
