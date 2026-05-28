/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:13:32 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:24:26 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutputSocket.hpp"
#include "ASocket.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

OutputSocket::OutputSocket(int socket_fd, Server& server):
	ASocket(server),
	ready(false),
	send_headers(true),
	ressourceFd(-1)
	//socketFd(dup(socket_fd))
{
	this->fd = dup(socket_fd);
	if (this->fd < 0)
		std::cerr << "could not duplicate socket_fd\n";
	else
		std::cout << "successfully duplicated socket_fd\n";
	//std::cout << "OutputSocket socketFd = " << socketFd << "\n";
}

OutputSocket::~OutputSocket()
{
}

void	OutputSocket::process()
{
	if (ready == false)
	{
		std::cout << "OutputSocket not yet ready to process\n";
		return ;
	}
	else
	{
		std::cout << "OutputSocket READY to process\n";
		if (send_headers == true)
		{
			if (send(this->fd, this->buf.data(), this->buf.size(), MSG_DONTWAIT | MSG_NOSIGNAL) < 0)
				std::cerr << "An error occured while sending data to server\n";
			else
				send_headers = false;
		}
		if (ressourceFd >= 0)
		{
			char	buffer[BUFSIZ];
			ssize_t nb_read;
			nb_read = read(this->ressourceFd, buffer, BUFSIZ);
			std::cout << "we read " << nb_read << " characters from the server\n";
			if (nb_read < 0)
				std::cerr << "An error occured while reading data from ressourceFd\n";
			else if (nb_read == 0)
					this->status = FAILURE;// here should then close the connection
			else
			{
				if (send(this->fd, buffer, nb_read, MSG_DONTWAIT | MSG_NOSIGNAL) < 0)
					std::cerr << "An error occured while sending data to server\n";
			}
		}
		else
		{
			this->status = FAILURE;// should close the connection
		}
		return ;
	}
}

static std::string status_phrase(uint16_t status_code);

void	OutputSocket::setup(const Response& resp)
{
	this->ressourceFd = resp.getResourceFd();

	std::string	status_string;
	std::stringstream ss;
	ss << resp.getStatus();
	ss >> status_string;

	this->buf.reserve(4096);

	this->buf += "HTTP/1.1";
	this->buf += " ";
	this->buf += status_string;
	this->buf += " ";
	this->buf += status_phrase(resp.getStatus());
	this->buf += "\r\n";

	this->buf += "Date: ";
	// insert date here !
	this->buf += "\r\n";
		
	this->buf += "Content-Type: text/html; charset=iso-8859-1\r\n";

	this->buf += "\r\n";

	this->ready = true;
}

static std::string status_phrase(uint16_t status_code)
{
	switch (status_code) {
		case 200: return "OK";
		case 404: return "Not Found";
		default: return "";
	}
}

