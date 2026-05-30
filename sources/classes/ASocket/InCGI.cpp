/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InCGI.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:24:47 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 11:42:27 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InCGI.hpp"
#include "ASocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <fcntl.h>
#include <unistd.h>

InCGI::InCGI(int fd, std::string& input_buffer, Server& server):
	ASocket(server),
	input_buffer(input_buffer)
{
	this->fd = fd;
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
		this->status = FAILURE;
}

void	InCGI::process()
{
	if (this->input_buffer.size() > 0)
	{
		ssize_t nb_write = write(this->fd, this->input_buffer.data(), this->input_buffer.size());
		if (nb_write < 0)
			return;
		else
			this->input_buffer = std::string(this->input_buffer, nb_write);
	}
}

InCGI::~InCGI()
{
}
