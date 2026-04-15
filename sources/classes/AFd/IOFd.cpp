/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:36:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOFd.hpp"
#include "AFd.hpp"
#include "Server.hpp"
#include "abnf.hpp"
#include "default_pages.hpp"
#include "status.hpp"
#include <cctype>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>


IOFd::IOFd(int fd, Server& server):
	AFd(server),
	state(0)
{
	this->fd = fd;
	IOFd::process_functions[0] = &IOFd::process_method;
	IOFd::process_functions[1] = &IOFd::process_uri;
	IOFd::process_functions[2] = &IOFd::process_version;
	IOFd::process_functions[3] = &IOFd::process_header;
	IOFd::process_functions[4] = &IOFd::process_body;
	IOFd::process_functions[5] = &IOFd::process_abort;
}

IOFd::~IOFd()
{
	//close(this->fd);
}

void IOFd::process()
{
	char buf[BUFSIZ];
	ssize_t nb_read = read(this->fd, buf, BUFSIZ);
	if (nb_read < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "read: " << error_msg << "\n";
	}
	else if (nb_read == 0)
	{
		this->server.remove(this);
	}
	else
	{
		std::string str(buf, nb_read);
		(this->*process_functions[this->state])(str, 0);
		if (this->fail())
			return ;
	}
	std::cout << *this << "\n";
}

void	IOFd::process_method(std::string& str, size_t pos)
{
	(void) pos; // this function should always be called with pos 
	// being 0 !
	size_t space_pos = str.find(' ');
	if (space_pos == str.npos)
	{
		// no space found adding everything in the 'method'
		this->method.append(str);
		if (this->method.size() > IOFD_MAX_SIZE ||
			this->method.find_first_not_of(ABNF_UPPER))
		{
			// here we detected an error
			// we should send the 'BAD request response to the
			// client, and then clear ressources for this connection
			int ret = send(this->fd, ERROR_PAGE_400, sizeof(ERROR_PAGE_400), MSG_DONTWAIT | MSG_NOSIGNAL);
			if (ret < 0)
			{
				std::cout << "error while sending error page back to client\n";
			}
			this->status = FAILURE;
		}
	}
}

void	IOFd::process_uri(std::string& str, size_t pos)
{
	(void) str;
	(void) pos;
}

void	IOFd::process_version(std::string& str, size_t pos)
{
	(void) str;
	(void) pos;
}

void	IOFd::process_header(std::string& str, size_t pos)
{
	(void) str;
	(void) pos;
}


void	IOFd::process_body(std::string& str, size_t pos)
{
	(void) str;
	(void) pos;
}

void	IOFd::process_abort(std::string& str, size_t pos)
{
	(void) str;
	(void) pos;
}


std::ostream& operator<<(std::ostream& os, const IOFd& iofd)
{
	os << "method: '" << iofd.method << "'\n";
	return (os);
}
