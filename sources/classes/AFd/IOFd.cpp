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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cwctype>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>


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
}

IOFd::~IOFd()
{
}

const std::string					&IOFd::getMethod(void) const { return(this->method); }
const std::string					&IOFd::getUri(void) const { return(this->uri); }
const std::string					&IOFd::getVersion(void) const { return(this->version); }
const std::vector< std::string >	&IOFd::getHeader(void) const { return(this->header); }
const std::vector<unsigned char>	&IOFd::getBody(void) const { return(this->body); }



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
		size_t	pos = 0;
		while (pos < str.size())
		{
			(this->*process_functions[this->state])(str, pos);
			if (this->fail())
				return ;
		}

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

void	IOFd::process_method(std::string& str, size_t& pos)
{ 
	size_t space_pos = str.find(' ', 0);
	if (space_pos == str.npos)
	{
		// no space found: add everything in the 'method' field
		this->method.append(str);
		if (check_method(this->method) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		pos = str.size();
	}
	else
	{
		this->method.append(str, pos, space_pos - pos);
		if (check_method(this->method) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		else
		{
			this->state++;
			//(this->*process_functions[this->state])(str, space_pos + 1);
			pos = space_pos + 1;
			return ;
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

static int	check_uri(std::string& uri);
static void	clear_uri(std::string& uri);

// goal: should fill and check URI
// - reject any uri containing invalid char, like newline etc.
// - should also remove .. and . for the URI before going to next step
// ?? do we treat url encoding and decoding ? like '/hi%20you.html' should
// be transformed to '/hi 20yo.html' that's some question we need to ask
void	IOFd::process_uri(std::string& str, size_t& pos)
{
	// std::cout << "in process uri\n";
	size_t space_pos = str.find(' ', pos);
	size_t crlf = str.find("\r\n", pos);
	size_t lf = str.find("\n", pos);
	size_t delim = std::min(space_pos, std::min(crlf, lf));
	if (delim == std::string::npos)
	{
		this->uri.append(str, pos, str.size() - pos);
		if (check_uri(this->uri) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		pos = str.size();
	}
	else
	{
		if (delim > pos)
			this->uri.append(str, pos, delim - pos);
		if (check_uri(this->uri) || this->uri == "")
			return (send_bad_request(this->fd, this->status));
		clear_uri(this->uri);
		this->state++;
		pos = delim;
	}
	return ;
}

// what is an invalid uri ?
// if size is too large
// if it contains other than allowed characters
static int	check_uri(std::string& uri)
{
	if (uri.size() > IOFD_MAX_SIZE ||
		uri.find_first_not_of(ABNF_PATH_ABEMPTY "[]{}<>?#") != std::string::npos
	)
		return (FAILURE);
	if (uri.size() > 0)
	{
		if (uri.at(0) != '/')
			return (FAILURE);
	}
	return (SUCCESS);
}

void split(std::vector<std::string>& res, std::string& src, std::string charset)
{
	size_t	start = 0;
	size_t	end = 0;

	while (start != std::string::npos && end != std::string::npos)
	{
		start = src.find_first_not_of(charset, end);
		end = src.find_first_of(charset, start);
		if (start == std::string::npos)
			return ;
		else if (end == std::string::npos)
		{
			res.push_back(std::string(src, start, src.size() - start));
			return ;
		}
		else
		{
			res.push_back(std::string(src, start, end - start));
		}
	}
}

// this function should clear uri, by removing '..'
// like '/hi/../you' into '/you'
static void	clear_uri(std::string& uri)
{
	std::vector<std::string>	splitted;
	bool						end_by_slash;
	std::vector<std::string>	transformed;

	if (uri[uri.size() - 1] == '/')
		end_by_slash = true;

	split(splitted, uri, "/");
	for (size_t i = 0; i < splitted.size(); i++)
	{
		if (splitted[i] == ".")
			;
		else if (splitted[i] == ".." && transformed.size() > 0)
			transformed.pop_back();
		else if (splitted[i] == "..")
			;
		else
			transformed.push_back(splitted[i]);
	}
	uri = "";
	for (size_t i = 0; i < transformed.size(); i++)
	{
		uri += '/';
		uri += transformed[i];
	}
	if (transformed.size() == 0 || end_by_slash == true)
		uri += '/';
}

static int	check_version(const std::string& method, const std::string& version);

// here function to process version part of the request
// should look something like: HTTP/*[DIGITS].*[DIGITS] or nothing 
// with a 'GET' method
// condition of failed version:
//	- version field greater than 2048 char (arbitrary size)
//	- version number not supported ?
//	- 
void	IOFd::process_version(std::string& str, size_t& pos)
{
	// std::cout << "in process version\n";
	size_t	crlf = str.find("\r\n", pos);
	size_t	lf = str.find("\n", pos);
	size_t	delim = std::min(crlf, lf);
	if (delim == str.npos)
	{
		this->version.append(str, pos, str.size() - pos);
		if (this->version.size() > IOFD_MAX_SIZE)
			return (send_bad_request(this->fd, this->status));
		pos = str.size();
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

		if (check_version(this->method, this->version) != SUCCESS)
			return (send_bad_request(this->fd, this->status));
		this->state++;
		pos = until;
	}
}

// should begin with 'HTTP/'
// and then two number separated by a '.'
// or should be empty with a 'GET' method
static int	check_version(const std::string& method, const std::string& version)
{
	if (version.size() > IOFD_MAX_SIZE)
		return (FAILURE);

	if (version == "")
	{
		if (method != "GET")
			return (FAILURE);
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

// here depending on the version, it should exepct no header 
// maybe header should be in key-value pairs ? like:
// std::map<std::string, std::string>, but a map is annoying because it
// does not allow duplicate keys, and keys can be duplicate
// so maybe more an std::vector of std::pairs of std::string, std::string
// maybe this is better this way ? i think we will go this way
// but its so much verbose ...
// or a std::map<std::string, std::vector<std::string>>
// this ones gives flexibility, but also very verbose
// so i do not really know ??
// so getting here, if version is empty, then it is a 'simple-request'
// if it is a simple request, then the method should be 'GET'
// and if it is the case, the request is complete, and should 
// be processed using only method and uri, then closed and all other
// ressources send should be ignored
void	IOFd::process_header(std::string& str, size_t& pos)
{
	// std::cout << "in process header\n";
	size_t	crlf ;
	size_t	lf;
	size_t	delim;
	size_t	until;
	if (this->version == "")
	{
		if (this->method == "GET")
		{
			// do stuff to stop parsing incoming data,
			// and process the request using only information
			// in 'method' and in 'uri'
			// return ...
		}
		else
		{
			return (send_bad_request(this->fd, this->status));
		}
	}

	while (pos < str.size())
	{
		crlf = str.find("\r\n", pos);
		lf = str.find("\n", pos);
		delim = std::min(crlf, lf);
		if (delim == std::string::npos) // no \r\n
			until = str.size();
		else if (delim == pos)
		{
			if (str[delim] == '\r')
				until = delim + 2;
			else
				until = delim + 1;
			// here we have a delim at begin,
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
				//(this->*process_functions[this->state])(str, until);
				pos = until;
				return;
			}
			else
			{
				std::string& last_line = this->header[this->header.size() - 1];
				if (last_line != "" &&
					last_line[last_line.size() - 1] == '\n')
				{
					this->state++;
					//(this->*process_functions[this->state])(str, until);
					pos = until;
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
				this->header.push_back(std::string(str, pos, until - pos));
		}
		pos = until;
	}
}


void	IOFd::process_body(std::string& str, size_t& pos)
{
	// std::cout << "in process body\n";
	// should reserve size of body right here because it should be known !
	// this->body.push_back(str.data(), pos, str.size() - pos);
	if (pos < str.size())
		this->body.insert(this->body.end(), str.begin() + pos, str.end());
	pos = str.size();
}

void	IOFd::process_skip_sp(std::string& str, size_t& pos)
{
	// std::cout << "in process skip spaces\n";
	size_t	non_sp_pos = str.find_first_not_of(" ", pos);
	if (non_sp_pos == str.npos)
		return ;
	this->state++;
	pos = non_sp_pos;
	return ;
}

std::ostream& operator<<(std::ostream& os, std::vector<unsigned char> data)
{
	for (size_t	i = 0; i < data.size(); i++)
	{
		char c;
		if (std::isprint(data.at(i)) )//|| std::iswspace(data.at(i)))
			c = static_cast<char>(data.at(i));
		else
			c = '.';
		os << c;
	}
	return (os);
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
	os << "headerlines: (nb headerlines: " << iofd.header.size() << ")\n";
	for (size_t i = 0; i < iofd.header.size(); i++)
	{
		os << iofd.header[i];
	}
	os << "----------------------\n";
	os << "body:\n";
	os << iofd.body;
	os << "\n";
	return (os);
}

