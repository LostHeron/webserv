/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InCGI.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:24:47 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 13:37:07 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InCGI.hpp"
#include "ASocket.hpp"
#include "Server.hpp"
#include "error.hpp"
#include "Connection.hpp"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cerrno>

InCGI::InCGI(int fd, size_t bodySize, std::string& newInputBuffer, Connection* connection):
	ASocket(connection),
	nbToSend(bodySize),
	nbSent(0),
	inputBuffer(newInputBuffer)
{
	this->fd = dup(fd); 
	if (this->fd < 0)
	{
		// TODO DANGER, what happens if dup fails ?
		// throw an error ?
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
	{
		int error_value = errno;
		logerror("error_value", error_value);
		//this->status = FAILURE;
	}
	if (fcntl(this->fd, F_SETFD, FD_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("error_value", error_value);
		//this->status = FAILURE;
	}
}

static void	sendDataCGI(std::string& buffer, int fd, size_t& nbSent, size_t& nbToSend, int& status);

void	InCGI::process()
{
	#ifdef DEBUG
	std::cout << "In InCGI process\n";
	#endif
	/*
	if (this->status != SUCCESS)
		return ;
	*/
	if (this->connection->isChunked() == true)
	{
		if (this->inputBufferChunk == "")
		{
			this->inputBufferChunk = this->connection->getChunk().getBuffer();
		}
		if (this->inputBufferChunk != "")
			sendDataCGI(this->inputBufferChunk, this->fd, this->nbSent, this->nbToSend, this->status);
	}
	else
	{
		sendDataCGI(this->inputBuffer, this->fd, this->nbSent, this->nbToSend, this->status);
	}
}

static void	sendDataCGI(std::string& buffer, int fd, size_t& nbSent, size_t& nbToSend, int& status)
{
	if (buffer.size() > 0)
	{
		size_t	tmp_size;
		if (nbSent + buffer.size() > nbToSend)
			tmp_size = nbToSend - nbSent;
		else
			tmp_size = buffer.size();

		if (tmp_size > 0)
		{
			ssize_t nb_write = write(fd, buffer.data(), tmp_size);
			if (nb_write < 0)
			{
				int errno_value = errno;
				logerror("write", errno_value);
				std::cerr << "InCgi could not wrote to process\n";
				return;
			}
			else
			{
				#ifdef DEBUG
				std::cout << "-->ACTION: InCgi wrote " << nb_write << " byte to pipe\n";
				#endif
				nbSent += nb_write;
				buffer = std::string(buffer, nb_write);
				if (nbSent >= nbToSend)
				{
					status = FINISH;
					close(fd);
					fd = -1;
				}

			}
		}
	}
}

InCGI::~InCGI()
{
	#ifdef DEBUG
	std::cout << "In INCGI DESTRUCTOR\n";
	#endif
}
