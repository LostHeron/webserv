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
#include "status.hpp"
#include "error.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

OutCGI::OutCGI(int fd, Server& server):
	ASocket(server),
	state(0)
{
	this->fd = dup(fd); // TODO DANGER, what happens if the DUP FAILS ?
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

void update_buffer(int fd, std::string& cgi_out_buffer);

void OutCGI::process()
{
	update_buffer(this->fd, this->cgi_out_buffer);
	size_t	start = 0;
	if (this->state == 0)
	{
		this->process_headers(start);
	}
	return ;
}

void OutCGI::process_headers(size_t &start)
{
	while (start < this->cgi_out_buffer.size())
	{
		if (fill_last_line(this->cgi_out_buffer, this->last_line, start, this->state) == STOP)
		{
			break;
		}

		if (check_last_line(this->last_line) != SUCCESS)
		{
			// ok must return here and kill the process ?
			// return (send_bad_request(this->fd, this->status));
		}

		if (this->last_line.size() > 0 && this->last_line[last_line.size() - 1] == '\n')
		{
			/*
			add_line_headers(this->last_line, this->headers);
			if (check_headers(this->headers) != SUCCESS)
				return (send_bad_request(this->fd, this->status));
			*/
			this->last_line.clear();
		}
	}
}

void update_buffer(int fd, std::string& cgi_out_buffer)
{
	if (cgi_out_buffer == "")
	{
		char buf[BUFSIZ];
		ssize_t nb_read = read(fd, buf, BUFSIZ);
		if (nb_read < 0)
		{
			int errno_value = errno;
			logerror("read", errno_value);
		}
		else
		{
			cgi_out_buffer = std::string(buf, nb_read);
			std::cout << "OutCgi read " << nb_read << " bytes\n";
			std::cout << "OutCgi buffer = '" << cgi_out_buffer << "'\n";
		}
	}
}

