/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutCGI.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:21:38 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 17:38:57 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutCGI.hpp"
#include "HTTPStatus.hpp"
#include "HeadersBuilder.hpp"
#include "InputSocket.hpp"
#include "sockets.hpp"
#include "status.hpp"
#include "error.hpp"
#include "typedef.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

OutCGI::OutCGI(int fd, InputSocket& is, OutputSocket& os, Server& server):
	ASocket(server),
	is(is),
	os(os),
	state(0)
{
	this->fd = dup(fd); // TODO DANGER, what do we do if the DUP FAILS ?
	if (this->fd < 0)
	{
		// TODO DANGER, what happens if dup fails ?
		// throw an error ?
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
		this->status = FAILURE;
}

OutCGI::~OutCGI()
{
}


void OutCGI::process()
{
	if (this->status == FINISH)
		return ;
	this->update_buffer();
	size_t	start = 0;
	if (this->state == 0)
	{
		this->process_headers(start);
	}
	if (this->state == 1)
		this->process_body(start);
	if (start >= this->cgi_out_buffer.size())
		this->cgi_out_buffer.clear();
	return ;
}

static int	check_headers(string_map& headers)
{
	if (headers.count("content-type") == 0)
		return (FAILURE);
	return (SUCCESS);
}

void OutCGI::process_headers(size_t &start)
{
	while (start < this->cgi_out_buffer.size())
	{
		if (fill_last_line(this->cgi_out_buffer, this->last_line, start, this->state) == STOP)
		{
			if (check_headers(this->headers) != SUCCESS)
				return (setup_response(this->status, HTTPStatus::S_ERR + HTTPStatus::INTERNAL, this->is, this->os));
			// here we will leave this function, so we should write all header informations to
			// buffer of os
			HeadersBuilder b;
			b.initialize()
				.buildStatusLine("HTTP/1.1", 200)
				.buildDate();
			for (string_map::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
			{
				b.buildHeaderKeyVecValue(it->first, it->second);
			}
			b.buildCRLF();
			this->os.getOutputBuffer() = b.build();
			//this->state++;
			break;
		}

		if (check_last_line(this->last_line) != SUCCESS)
		{
			return (setup_response(this->status, HTTPStatus::S_ERR + HTTPStatus::INTERNAL, this->is, this->os));
		}

		if (this->last_line.size() > 0 && this->last_line[last_line.size() - 1] == '\n')
		{
			add_line_headers(this->last_line, this->headers);
			this->last_line.clear();
		}
	}
}

void OutCGI::process_body(size_t &start)
{
	if (start > 0)
	{
		this->cgi_out_buffer = std::string(this->cgi_out_buffer, start);
		start = 0;
	}
	if (this->os.getOutputBuffer() == "" && this->cgi_out_buffer != "")
	{
		this->os.getOutputBuffer() = this->cgi_out_buffer;
		this->cgi_out_buffer.clear();
	}
	return;
}

void OutCGI::update_buffer()
{
	if (this->cgi_out_buffer == "")
	{
		char buf[BUFSIZ];
		ssize_t nb_read = read(this->fd, buf, BUFSIZ);
		if (nb_read < 0)
		{
			int errno_value = errno;
			logerror("read", errno_value);
		}
		else if (nb_read == 0)
		{
			// if nb_read is 0 and we still in state = 0
			// then send internal
			if (this->state == 0)
				return (setup_response(status, HTTPStatus::S_ERR + HTTPStatus::INTERNAL, is, os));
			else
				this->os.getIsLastBuffer() = true;
		}
		else
		{
			cgi_out_buffer = std::string(buf, nb_read);
			std::cout << "OutCgi read " << nb_read << " bytes\n";
			std::cout << "OutCgi buffer = '" << cgi_out_buffer << "'\n";
		}
	}
}
