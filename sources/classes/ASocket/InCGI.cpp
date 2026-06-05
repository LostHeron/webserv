/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InCGI.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:24:47 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 17:39:44 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InCGI.hpp"
#include "ASocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cerrno>

InCGI::InCGI(int fd, std::string& input_buffer, Connection* connection):
	ASocket(connection),
	input_buffer(input_buffer)
{
	this->fd = dup(fd); 
	if (this->fd < 0)
	{
		// TODO DANGER, what happens if dup fails ?
		// throw an error ?
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
		this->status = FAILURE;
}

#include "error.hpp"

void	InCGI::process()
{
	if (this->input_buffer.size() > 0)
	{
		ssize_t nb_write = write(this->fd, this->input_buffer.data(), this->input_buffer.size());
		if (nb_write < 0)
		{
			int errno_value = errno;
			logerror("write", errno_value);
			std::cerr << "InCgi could not wrote to process\n";
			return;
		}
		else
		{
			std::cout << "-->ACTION: InCgi wrote " << nb_write << " byte to pipe\n";
			this->input_buffer = std::string(this->input_buffer, nb_write);
		}
	}
}

InCGI::~InCGI()
{
}
