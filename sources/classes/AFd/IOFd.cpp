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
	ssize_t nb_read = recv(this->fd, buf, BUFSIZ, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (nb_read < 0)
	{
		// error happened
		std::string error_msg(strerror(errno));
		std::cerr << "read: " << error_msg << "\n";
		this->status = FAILURE;
	}
	else if (nb_read == 0)
	{
		// other end closed the connection
		this->status = FAILURE;
	}
	else
	{
		// nomal behaviour

		// creating string from buffer
		std::string str(buf, nb_read);

		// processing buffer based on current state of IOFd;
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

static int	check_method(std::string& method);

void	IOFd::process_method(std::string& str, size_t pos)
{ 
	// this function should always be called with pos
	// being 0 !
	(void) pos; 

	size_t space_pos = str.find(' ', 0);
	if (space_pos == str.npos)
	{
		// no space found: add everything in the 'method' field
		this->method.append(str);
		if (check_method(this->method) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
	}
	else
	{
		this->method.append(str, pos, space_pos - pos);
		if (check_method(this->method) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		else
		{
			this->state++;
			(this->*process_functions[this->state])(str, space_pos + 1);
		}
	}
}

static int	check_method(std::string& method)
{
	if (method == "" ||
		method.size() > IOFD_MAX_SIZE ||
		method.find_first_not_of(ABNF_UPPER) != std::string::npos)
		return (FAILURE);
	return (SUCCESS);
}

// should check rules for valid URI and not valid URI
// like reject any uri containing invalid char, like newline etc.
// should also remove .. and . for the URI before going to next step
void	IOFd::process_uri(std::string& str, size_t pos)
{
	std::cout << "in process uri\n";
	size_t space_pos = str.find(' ', pos);
	size_t crlf = str.find("\r\n", pos);
	size_t lf = str.find("\n", pos);
	size_t delim = std::min(space_pos, std::min(crlf, lf));
	if (delim == std::string::npos)
	{
		this->uri.append(str, pos, str.size() - pos);
		// here should check that the uri contains only valid characters
		// and that its size is valid!
		if (this->uri.size() > IOFD_MAX_SIZE)
			return (send_bad_request(this->fd, this->status));
	}
	else
	{
		if (delim > pos)
			this->uri.append(str, pos, delim - pos);
		if (this->uri.find_first_not_of(ABNF_PATH_ABEMPTY) != std::string::npos)
			return (send_bad_request(this->fd, this->status));
		this->state++;
		(this->*process_functions[this->state])(str, delim);
	}
}

static int	check_version(const std::string& version);

// here function to process version part of the request
// should look something like: HTTP/*[DIGITS].*[DIGITS] or nothing 
// condition of failed version:
//	- version field greater than 2048 char (arbitrary size)
//	- version number not supported ?
//	- 
void	IOFd::process_version(std::string& str, size_t pos)
{
	std::cout << "in process version\n";
	size_t	crlf = str.find("\r\n", pos);
	size_t	lf = str.find("\n", pos);
	size_t	delim = std::min(crlf, lf);
	if (delim == str.npos)
	{
		// no new line found !
		this->version.append(str, pos, str.size() - pos);
		if (this->version.size() > IOFD_MAX_SIZE)
			return (send_bad_request(this->fd, this->status));
		//	if (version invalid)
		//		return (send_bad_request(this->fd, this->status));
	}
	else
	{
		size_t	until;
		if (str[delim] == '\r')
			until = delim + 2;
		else
			until = delim + 1;
		this->version.append(str, pos, delim - pos);

		size_t	trailing_space_pos = this->version.find_last_not_of(" ") + 1;
		this->version.erase(trailing_space_pos, version.size() - trailing_space_pos);

		if (check_version(this->version) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		//	if (version invalid)
		//		return (send_bad_request(this->fd, this->status));
		//	else
		//	{
		this->state++;
		(this->*process_functions[this->state])(str, until);
		//	}

	}
}

// should begin with HTTP
// should be followed by a /
// and then two number separated by a '.'
// or should be empty
static int	check_version(const std::string& version)
{
	if (version.size() > IOFD_MAX_SIZE)
		return (FAILURE);
	if (version == "")
	{
		// TODO HERE IF NO VERSION WAS GIVEN:
		// CHECK THAT THE KEYWORD IS GET 
		// OR REFUSE WITH BAD REQUEST 
		return (SUCCESS);
	}
	if (std::strncmp(version.c_str(), "HTTP/", 5) != 0)
		return (FAILURE);

	size_t	dot_pos = version.find_first_not_of(ABNF_DIGIT, 5);
	if (dot_pos == std::string::npos || version.at(dot_pos) != '.')
		return (FAILURE);
	long major = std::strtol(version.c_str() + 5, NULL, 10);
	if (major != 1)
		return (FAILURE);
	// TODO return 505 version not handled by the server

	size_t	end_pos = version.find_first_not_of(ABNF_DIGIT, dot_pos + 1);
	if (end_pos != std::string::npos)
		return (FAILURE);
	long minor = std::strtol(version.c_str() + dot_pos, NULL, 10);
	if (minor != 0 && minor != 1 && minor != 2 && minor != 3)
		return (FAILURE);
	// TODO return 505 version not handled by the server
	return (SUCCESS);
}

void	IOFd::process_header(std::string& str, size_t pos)
{
	std::cout << "in process header\n";
	size_t	crlf ;
	size_t	lf;
	size_t	delim;
	size_t	until;

	while (pos < str.size())
	{
		crlf = str.find("\r\n", pos);
		lf = str.find("\n", pos);
		delim = std::min(crlf, lf);
		if (delim == str.npos) // no \r\n
			until = str.size();
		else if (delim == pos)
		{
			//here we have a delim at begin,
			// we need to check if previous last char
			// stored is also a new line and if so,
			// next data must go into the body section
			if (header.size() == 0)
			{
				// does it instantly means we are go in the body ??
				// if (header not valid)
			// {
			//		do stuff;
			// }
				this->state++;
				(this->*process_functions[this->state])(str, pos);
				return;
			}
			else
			{
				std::string& last_line = this->header[this->header.size() - 1];
				if (last_line == "" ||
					last_line[last_line.size() - 1] != '\n')
				{
					if (str[delim] == '\r')
						until = delim + 2;
					else
						until = delim + 1;
				}
				else
				{
					this->state++;
					(this->*process_functions[this->state])(str, pos);
					return;
				}
			}
		}
		else
		{
			if (str[delim] == '\r')
				until = delim + 2;
			else
				until = delim + 1;
		}
		if (header.size() == 0)
			this->header.push_back(std::string(str, pos, until - pos));
		else
		{
			std::string& last_line = this->header[this->header.size() - 1];
			if (last_line.size() == 0 ||
				last_line[last_line.size() - 1] != '\n')
				last_line.append(str, pos, until - pos);
			else
			{
				this->header.push_back(std::string(str, pos, until - pos));
			}
		}
		pos = until;
	}
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
	os << "\n----------------------\n";
	os << "headerlines:\n";
	for (size_t i = 0; i < iofd.header.size(); i++)
	{
		os << iofd.header[i];
	}
	os << "\n----------------------\n";
	os << "\n";
	return (os);
}
