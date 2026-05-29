/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_request.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:07:29 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:09:22 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "RequestFactory.hpp"
#include "VirtualHost.hpp"
#include "OutputSocket.hpp"

void	InputSocket::process_request(size_t& pos)
{
	std::string requested_server_name;
	// ach: build arequest (GET/POST/DEL...) from previoulsy fullfilled iofd
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

	// this function of OutputSocket should
	// set the ressource fd to the correct output
	// set the status code to correct stuff
	// and then when the ASocket corresponding to that OutputSocket
	// is processed, it should, check if some flag saying processing 
	// is ok it should process the request, in other case, just go away
	// when processing, it should send headers in the first place,
	// and then read from the ressource fd if it is set to a value greater
	// than 1.
	// and when this is done, hm, what should we do ?
	// and what about cgi, because in this model, we have not yet created
	// process the body, and before processing de ressourceFd, the body should've 
	// been passed to the underlying process, so ...

	// this should be if no cgi :
	static_cast<OutputSocket*>(this->associatedSocket)->setup(resp);
	// and here if it is cgi, then setup the InputCgi and OutputCgi

	delete req;
	
	/*
	if (resp.getResourceFd() != -1)
		close(resp.getResourceFd());
	*/
	this->state++;
	if (pos < this->input_buffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}

