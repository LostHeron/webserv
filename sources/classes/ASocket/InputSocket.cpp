/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:52:27 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "ASocket.hpp"
#include "OutputSocket.hpp"
#include "RequestFactory.hpp"
#include "Server.hpp"
#include "VirtualHost.hpp"
#include "abnf.hpp"
#include "default_pages.hpp"
#include "status.hpp"
#include <cctype>
#include <cstddef>
#include <stdint.h>
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

InputSocket::InputSocket(int fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server):
	ASocket(server),
	state(0),
	local_port(local_port),
	peer_port(ntohs(addr.sin_port))
{
	uint32_t addrh = (addr.sin_addr.s_addr);
	for (int i = 0; i < 4; i++)
	{
		this->addr[i] = ( reinterpret_cast<uint8_t *>(&addrh) )[i];
	}
	this->fd = fd;
	InputSocket::process_functions[0] = &InputSocket::process_method;
	InputSocket::process_functions[1] = &InputSocket::process_skip_sp;
	InputSocket::process_functions[2] = &InputSocket::process_uri;
	InputSocket::process_functions[3] = &InputSocket::process_skip_sp;
	InputSocket::process_functions[4] = &InputSocket::process_version;
	InputSocket::process_functions[5] = &InputSocket::process_header;
	InputSocket::process_functions[6] = &InputSocket::process_request;
	InputSocket::process_functions[7] = &InputSocket::process_body;
}

InputSocket::~InputSocket()
{
}

const std::string					&InputSocket::getMethod(void) const { return(this->method); }
const std::string					&InputSocket::getUri(void) const { return(this->uri); }
const std::string					&InputSocket::getVersion(void) const { return(this->version); }
const string_map					&InputSocket::getHeader(void) const { return(this->header); }
const std::vector<unsigned char>	&InputSocket::getBody(void) const { return(this->body); }

void InputSocket::process()
{
	char buf[BUFSIZ];
	// ok maybe add a check here before to read, if the buffer is empty,
	// and by buffer i mean the 'str' variable down below that
	// should be then made static so it can be accessed over different calls
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

		// processing buffer based on current state of InputSocket;
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

void	InputSocket::process_method(std::string& str, size_t& pos)
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
void	InputSocket::process_uri(std::string& str, size_t& pos)
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

	end_by_slash = false;
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
void	InputSocket::process_version(std::string& str, size_t& pos)
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

void	InputSocket::process_request(std::string& str, size_t& pos)
{
	(void) str;
	(void) pos;
	std::string requested_server_name;
	// ach: build arequest (GET/POST/DEL...) from previoulsy fullfilled iofd
	if (this->header.count("host"))
	{
		if (this->header["host"].size() > 0)
			requested_server_name = this->header["host"].at(0);
		else
			requested_server_name = "";
	}
	else
		requested_server_name = "";
	const VirtualHost& vhost = this->server.getHostList().getHost(this->local_port, requested_server_name);
	RequestFactory facto(*this, vhost);//, VirtualHost &vhost;
	ARequest *req = facto.createElement();

	// ach: execute request building response metadata, then Jules will handle the Client transmission
	Response resp = req->execute();

	// this function of OutputSocket should
	// set the ressource fd to the correct output
	// set the status code to correct stuff
	// and then when the ASocket corresponding to that OutputSocket
	// is processed, it should, check if some flag saying processing 
	// is ok it should process the request, in other case, just go away
	// when processing, it should send headers in the first place,
	// and then read from the ressource fd if it is set to a value greater
	// than 1.
	// and when this is done, hm, what should we do ?
	// and what about cgi, because in this model, we have not yet created
	// process the body, and before processing de ressourceFd, the body should've 
	// been passed to the underlying process, so ...
	static_cast<OutputSocket*>(this->associatedSocket)->setup(resp);

	delete req;
	
	/*
	if (resp.getResourceFd() != -1)
		close(resp.getResourceFd());
	*/
	this->state++;
	return ;
}


void	InputSocket::process_body(std::string& str, size_t& pos)
{
	// std::cout << "in process body\n";
	// should reserve size of body right here because it should be known !
	// this->body.push_back(str.data(), pos, str.size() - pos);
	if (pos < str.size())
		this->body.insert(this->body.end(), str.begin() + pos, str.end());
	pos = str.size();
}

void	InputSocket::process_skip_sp(std::string& str, size_t& pos)
{
	// std::cout << "in process skip spaces\n";
	size_t	non_sp_pos = str.find_first_not_of(" ", pos);
	if (non_sp_pos == str.npos)
		return ;
	this->state++;
	pos = non_sp_pos;
	return ;
}

size_t		getDelimPosition(const std::string& str, size_t start, const std::vector<std::string>& delims)
{
	size_t res = std::string::npos;

	for (size_t i = 0; i < delims.size(); i++)
	{
		size_t	tmp = str.find(delims.at(i), start);
		if (tmp < res)
			res = tmp;
	}
	return (res);
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

std::ostream& operator<<(std::ostream& os, const InputSocket& iofd)
{
	os << "connection: ";
	for (int i = 0; i < 4; i++)
	{
		os << static_cast<int>(iofd.addr[i]);
		if (i != 3)
			os << ".";
	}
	os << ":" << iofd.peer_port << "; ";
	os << "method: '" << iofd.method << "'; ";
	os << "uri: '" << iofd.uri << "'; ";
	os << "version: '" << iofd.version << "'; ";
	os << "\n----------------------\n";
	os << "headerlines: (nb headerlines: " << iofd.header.size() << ")\n";
	for (string_map::const_iterator l = iofd.header.begin(); l != iofd.header.end(); l++)
	{
		os << l->first << ": ";
		for (size_t i = 0; i < l->second.size(); i++)
		{
			os << "'" << l->second[i] << "', ";
		}
		os << "\n";
	}
	os << "----------------------\n";
	os << "body:\n";
	os << iofd.body;
	os << "\n";
	return (os);
}

