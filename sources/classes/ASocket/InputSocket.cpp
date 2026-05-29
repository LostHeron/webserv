/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:12:29 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "ASocket.hpp"
#include "OutputSocket.hpp"
#include "Server.hpp"
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
	InputSocket::process_functions[5] = &InputSocket::process_headers;
	InputSocket::process_functions[6] = &InputSocket::process_request;
	InputSocket::process_functions[7] = &InputSocket::process_body;
}

InputSocket::~InputSocket()
{
}

const std::string					&InputSocket::getMethod(void) const { return(this->method); }
const std::string					&InputSocket::getUri(void) const { return(this->uri); }
const std::string					&InputSocket::getVersion(void) const { return(this->version); }
const string_map					&InputSocket::getHeaders(void) const { return(this->headers); }

void	updateInputBuffer(std::string& input_buffer, int fd, int& status);

void InputSocket::process()
{
	updateInputBuffer(this->input_buffer, this->fd, this->status);
	if (this->status != SUCCESS)
		return ;

	size_t	position = 0;
	(this->*process_functions[this->state])(position);
	if (this->fail())
		return ;
	if (position >= this->input_buffer.size())
		this->input_buffer.clear();
	std::cout << *this << "\n";
}

void	updateInputBuffer(std::string& input_buffer, int fd, int& status)
{
	if (input_buffer == "")
	{
		char buf[BUFSIZ];
		ssize_t nb_read = recv(fd, buf, BUFSIZ, MSG_DONTWAIT | MSG_NOSIGNAL);
		if (nb_read < 0)
		{
			// error happened
			std::string error_msg(strerror(errno));
			std::cerr << "read: " << error_msg << "\n";
			status = FAILURE;
			return ;
		}
		else if (nb_read == 0)
		{
			status = FAILURE; 
			// rename this, it is not failure, but
			//	is used to make server clear ressources associated 
			//	with this InputSocket request and associated OutputSocket
			return ;
		}
		else
		{
			input_buffer = std::string(buf, nb_read);
		}
	}
}

void	send_bad_request(int fd, int& status)
{
	int ret = send(fd, ERROR_PAGE_400, sizeof(ERROR_PAGE_400), MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0)
	{
		std::cerr << "error while sending error page back to client\n";
	}
	status = FAILURE;
}

void	InputSocket::process_body(size_t& pos)
{
	if (pos != 0)
		this->input_buffer = std::string(this->input_buffer, pos);
	return ;
}

void	InputSocket::process_skip_sp(size_t& pos)
{
	// std::cout << "in process skip spaces\n";
	size_t	non_sp_pos = this->input_buffer.find_first_not_of(" ", pos);
	if (non_sp_pos == this->input_buffer.npos)
		return ;
	this->state++;
	pos = non_sp_pos;
	if (pos < this->input_buffer.size())
		(this->*process_functions[this->state])(pos);
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

std::ostream& operator<<(std::ostream& os, const InputSocket& inputSocket)
{
	os << "connection: ";
	for (int i = 0; i < 4; i++)
	{
		os << static_cast<int>(inputSocket.addr[i]);
		if (i != 3)
			os << ".";
	}
	os << ":" << inputSocket.peer_port << ";\n";
	os << "current buffer contains: '" << inputSocket.input_buffer << "'\n";
	os << "method: '" << inputSocket.method << "'; ";
	os << "uri: '" << inputSocket.uri << "'; ";
	os << "version: '" << inputSocket.version << "'; ";
	os << "\n----------------------\n";
	os << "headerlines: (nb headerlines: " << inputSocket.headers.size() << ")\n";
	for (string_map::const_iterator l = inputSocket.headers.begin(); l != inputSocket.headers.end(); l++)
	{
		os << l->first << ": ";
		for (size_t i = 0; i < l->second.size(); i++)
		{
			os << "'" << l->second[i] << "', ";
		}
		os << "\n";
	}
	os << "----------------------\n";
	/*
	os << "body:\n";
	os << inputSocket.body;
	os << "\n";
	*/
	return (os);
}

