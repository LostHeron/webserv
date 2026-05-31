/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_request.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:07:29 by jweber            #+#    #+#             */
/*   Updated: 2026/05/31 17:08:35 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "RequestFactory.hpp"
#include "ToOutSocket.hpp"
#include "VirtualHost.hpp"
#include "OutputSocket.hpp"
#include <sstream>
#include <sys/epoll.h>

static std::string status_phrase(uint16_t status_code);

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
	if (iscgi == true)
	{
		this->prepareCGI();
	}
	else
	{
		OutputSocket* os = static_cast<OutputSocket*>(this->associatedSocket);
		std::string&	output_buffer = os->getOutputBuffer();
		bool&			isLastBuffer = os->getIsLastBuffer();
		output_buffer += this->version;
		output_buffer += " ";

		std::string			status_string;
		std::stringstream	ss;
		ss << resp.getStatus();
		ss >> status_string;
		output_buffer += status_string;
		output_buffer += " ";
		
		output_buffer += status_phrase(resp.getStatus());
		output_buffer += "\r\n";


		output_buffer += "Date: ";
		output_buffer += "\r\n";
			
		/*
		output_buffer += "Content-Type: text/html; charset=iso-8859-1\r\n";
		output_buffer += "\r\n";
		*/

		output_buffer += "\r\n";
		if (resp.getResourceFd() < 0)
			isLastBuffer = true;
		else
		{
			ToOutSocket *tos = new ToOutSocket(resp.getResourceFd(), isLastBuffer, output_buffer, this->server);
			this->server.add(tos, EPOLLIN);
			this->associatedToOutSocket = tos;
		}
	}
	
	this->state++;
	if (pos < this->input_buffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}

static std::string status_phrase(uint16_t status_code)
{
	switch (status_code) {
		case 200: return "OK";
		case 404: return "Not Found";
		default: return "";
	}
}
