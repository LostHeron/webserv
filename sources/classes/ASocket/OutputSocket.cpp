/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:13:32 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:30:28 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutputSocket.hpp"
#include "ASocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cerrno>
#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

OutputSocket::OutputSocket(int socket_fd, Connection* connection):
	ASocket(connection),
	ressourceFd(-1),
	ready(false),
	isLastBuffer(false)
{
	this->fd = dup(socket_fd);
	if (this->fd < 0)
	{
		std::cerr << "could not duplicate socket_fd\n";
		this->status = FAILURE;
		// throw ??
	}
	else if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
	}
	else if (fcntl(this->fd, F_SETFD, FD_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
	}
	else
		std::cout << "successfully duplicated socket_fd\n";
}

OutputSocket::~OutputSocket()
{
	std::cout << "In Outputsocket destructor\n";
	if (this->ressourceFd >= 0)
		close(this->ressourceFd);
	this->ressourceFd = -1;
}

void	OutputSocket::setup(int newRessourceFd, const std::string& firstBuffer)
{
	this->ressourceFd = newRessourceFd;
	this->outputBuffer = firstBuffer;
	if (this->ressourceFd < 0)
		this->isLastBuffer = true;
	this->ready = true;
}

void	OutputSocket::process()
{
	std::cout << "in OutputSocket process()\n";
	if (this->ready == true && this->outputBuffer == "")
	{
		updateOutputBuffer();
	}
	if (this->outputBuffer.size() > 0)
	{
		ssize_t nb_send = send(this->fd, this->outputBuffer.data(), this->outputBuffer.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
		if (nb_send < 0)
			std::cerr << "An error occured while sending data to server\n";
		else
		{
			// used to transform this->buf = "salut toi le boss", 
			// whith nb_send = 2 to this->buf = "lut toi le boss"
			this->outputBuffer = std::string(this->outputBuffer, nb_send);
		}
	}
	if (this->outputBuffer.size() == 0 && this->isLastBuffer == true)
	{
		// TODO : this status set to FAILURE is
		// used to clean ressources associated with the ressource,
		// change name for better understanding of the meaning
		// like TERMINATE instead of FAILURE or something
		// or finish ?
		this->status = FAILURE; 
	}
}

void	OutputSocket::updateOutputBuffer()
{
	char buf[BUFSIZ];
	ssize_t nb_read = read(this->ressourceFd, buf, BUFSIZ);
	if (nb_read < 0)
	{
		int errno_value = errno;
		logerror("read", errno_value);
		this->status = FAILURE;
	}
	else if (nb_read == 0)
	{
		this->isLastBuffer = true;
	}
	else
	{
		this->outputBuffer = std::string(buf, nb_read);
	}
}


std::string& OutputSocket::getOutputBuffer()
{
	return (this->outputBuffer);
}

bool&			OutputSocket::getIsLastBuffer()
{
	return (this->isLastBuffer);
}

void			OutputSocket::end()
{
	this->isLastBuffer = true;
}

/*

#include "ARequest.hpp"
#include "Response.hpp"
#include <sstream>

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
*/
