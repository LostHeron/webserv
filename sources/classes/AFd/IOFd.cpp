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
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>


IOFd::IOFd(int fd, const struct sockaddr_in& addr, Server& server):
	AFd(server),
	state(0)
{
	this->port = ntohs(addr.sin_port);
	uint32_t addrh = (addr.sin_addr.s_addr);
	for (int i = 0; i < 4; i++)
	{
		this->addr[i] = ( reinterpret_cast<uint8_t *>(&addrh) )[i];
	}
	this->fd = fd;
	IOFd::process_functions[0] = &IOFd::process_method;
	IOFd::process_functions[1] = &IOFd::process_skip_sp;
	IOFd::process_functions[2] = &IOFd::process_uri;
	IOFd::process_functions[3] = &IOFd::process_skip_sp;
	IOFd::process_functions[4] = &IOFd::process_version;
	IOFd::process_functions[5] = &IOFd::process_header;
	IOFd::process_functions[6] = &IOFd::process_body;
	IOFd::process_functions[7] = &IOFd::process_abort;
}

IOFd::~IOFd()
{
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
		this->status = FAILURE;
		//this->server.remove(this);
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

void	send_bad_request(int fd, int& status)
{
	int ret = send(fd, ERROR_PAGE_400, sizeof(ERROR_PAGE_400), MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0)
	{
		std::cout << "error while sending error page back to client\n";
	}
	status = FAILURE;
}

void	IOFd::process_method(std::string& str, size_t pos)
{
	(void) pos; // this function should always be called with pos 
	// being 0 !
	size_t space_pos = str.find(' ', 0);
	if (space_pos == str.npos)
	{
		// no space found add: everything in the 'method' field
		this->method.append(str);
		if (this->method.size() > IOFD_MAX_SIZE ||
			this->method.find_first_not_of(ABNF_UPPER) != this->method.npos)
		{
			// Error sending bad request
			return (send_bad_request(this->fd, this->status));
		}
	}
	else
	{
		this->method.append(str, pos, space_pos - pos);
		if (this->method == "" ||
			this->method.find_first_not_of(ABNF_UPPER) != this->method.npos ||
			this->method.size() > IOFD_MAX_SIZE)
		{
			return (send_bad_request(this->fd, this->status));
		}
		else
		{
			// should go to step two !
			// so is step two objectives to get URI
			// or to skip SP anv HT ?
			// et est ce qu'on est strict ou on est large ??
			this->state++;
			(this->*process_functions[this->state])(str, space_pos + 1);
		}
	}
}

void	IOFd::process_uri(std::string& str, size_t pos)
{
	std::cout << "in process uri\n";
	size_t space_pos = str.find(' ', pos);
	if (space_pos == str.npos)
	{
		this->uri.append(str, pos, str.size() - pos);
		// here should check that the uri contains only valid characters
		// and that its size is valid!
		if (this->uri.size() > IOFD_MAX_SIZE)
			return (send_bad_request(this->fd, this->status));
	}
	else
	{
		this->uri.append(str, pos, space_pos - pos);
		//	here should check that the uri is valid !
		//	if (uri invalid)
		//		return (send_bad_request(this->fd, this->status));
		//	else
		//	{
			this->state++;
			(this->*process_functions[this->state])(str, space_pos);
		//	}
	}
}

void	IOFd::process_version(std::string& str, size_t pos)
{
	std::cout << "in process version\n";
	size_t	crlf = str.find("\r\n", pos);
	size_t	lf = str.find("\n", pos);
	size_t	delim = std::min(crlf, lf);
	if (delim == str.npos)
	{
		this->version.append(str, pos, str.size() - pos);
		// check validiry of header
	}
	else
	{
		this->version.append(str, pos, delim - pos);
		//	if (version invalid)
		//		return (send_bad_request(this->fd, this->status));
		//	else
		//	{
			this->state++;
			if (str[delim] == '\r')
				(this->*process_functions[this->state])(str, delim + 2);
			else
				(this->*process_functions[this->state])(str, delim + 1);
		//	}

	}
}

void	IOFd::process_header(std::string& str, size_t pos)
{
	std::cout << "in process header\n";
	(void) str;
	(void) pos;
}


void	IOFd::process_body(std::string& str, size_t pos)
{
	std::cout << "in process body\n";
	(void) str;
	(void) pos;
}

void	IOFd::process_abort(std::string& str, size_t pos)
{
	std::cout << "in process body\n";
	(void) str;
	(void) pos;
}

void	IOFd::process_skip_sp(std::string& str, size_t pos)
{
	std::cout << "in process skip spaces\n";
	size_t	non_sp_pos = str.find_first_not_of(" ", pos);
	if (non_sp_pos == str.npos)
		return ;
	this->state++;
	(this->*process_functions[this->state])(str, non_sp_pos);
}


std::ostream& operator<<(std::ostream& os, const IOFd& iofd)
{
	os << "connection: ";
	for (int i = 0; i < 4; i++)
	{
		os << static_cast<int>(iofd.addr[i]);
		if (i != 3)
			os << ".";
	}
	os << ":" << iofd.port << "; ";
	os << "method: '" << iofd.method << "'; ";
	os << "uri: '" << iofd.uri << "'; ";
	os << "version: '" << iofd.version << "'; ";
	os << "\n";
	return (os);
}
