/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 14:28:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "ASocket.hpp"
#include "InCGI.hpp"
#include "IsChildren.hpp"
#include "OutCGI.hpp"
#include "Pipe.hpp"
#include "Server.hpp"
#include "default_pages.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <cwctype>
#include <netinet/in.h>
#include <ostream>
#include <sys/epoll.h>
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
	peer_port(ntohs(addr.sin_port)),
	associatedInCgi(NULL),
	associatedOutCgi(NULL)
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
	if (associatedInCgi != NULL)
		this->server.remove(this->associatedInCgi);
	this->associatedInCgi = NULL;
	if (associatedOutCgi != NULL)
		this->server.remove(this->associatedOutCgi);
	this->associatedOutCgi = NULL;
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
		std::cout << "-->ACTION: InputSocket read " << nb_read << "bytes\n";
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
	else
	{
		std::cout << "-->ACTION: InputSocket does not read anything, buffer not empty\n";
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


void	InputSocket::prepareCGI()
{
	std::string script_name = "/home/jweber/goinfre/test.sh";
	std::ifstream file;
	file.open(script_name.c_str());
	if (file.fail())
	{
		std::cerr << "could not open script '" << script_name << "'\n";
		this->status = FAILURE;
		return ;
	}

	std::string line;
	std::getline(file, line);
	if (file.fail())
	{
		std::cerr << "could not read first line of script '" << script_name << "'\n";
		this->status = FAILURE;
		return ;
	}

	std::string path;
	std::vector<char *> args;
	if (std::strncmp(line.c_str(), "#!", 2) == 0)
	{
		path = std::string(line, 2);

		char *tmp;

		tmp = new char[path.size() + 1];
		std::memcpy(tmp, path.data(), path.size());
		tmp[path.size()] = '\0';
		args.push_back(tmp);

		tmp = new char[script_name.size() + 1];
		std::memcpy(tmp, script_name.data(), script_name.size());
		tmp[script_name.size()] = '\0';
		args.push_back(tmp);

		args.push_back(NULL);
	}
	else
	{
		path = script_name;

		char *tmp;

		tmp = new char[path.size() + 1];
		std::memcpy(tmp, path.data(), path.size());
		tmp[path.size()] = '\0';
		args.push_back(tmp);

		args.push_back(NULL);
	}

	Pipe toCGI;
	Pipe fromCGI;

	int pid = fork();
	if (pid < 0)
	{
		logerror();
		this->status = FAILURE;
		return ;
	}

	if (pid == 0)
	{
		//here is the child !
		try
		{
			toCGI.closeWriteEnd();
			fromCGI.closeReadEnd();
			if (dup2(toCGI.getReadEnd(), STDIN_FILENO) < 0)
			{
				// handle error here
			}
			if (dup2(fromCGI.getWriteEnd(), STDOUT_FILENO) < 0)
			{
				// handle error here
			}
			fromCGI.closeWriteEnd();
			toCGI.closeReadEnd();

			std::vector<std::string>	vec_envp;
			this->updateCgiEnvp(vec_envp);

			std::vector< char * > formatted_envp;
			formatted_envp.reserve(vec_envp.size() + 1);
			for (size_t i = 0; i < vec_envp.size(); i++)
			{
				char *tmp = new char[vec_envp.at(i).size() + 1];
				std::memcpy(tmp, vec_envp.at(i).data(), vec_envp.at(i).size());
				tmp[vec_envp.at(i).size()] = '\0';
				formatted_envp.push_back(tmp);
			}
			formatted_envp.push_back(NULL);


			char **envp = static_cast<char **>(formatted_envp.data());
			for (size_t i = 0; envp[i] != NULL; i++)
			{
				std::cerr << envp << "\n";
			}

			execve(path.c_str(), args.data(), envp);
			logerror();
			for (size_t i = 0; i < formatted_envp.size(); i++)
			{
				delete [] formatted_envp.at(i);
			}
			for (size_t i = 0; i < args.size(); i++)
			{
				delete [] args.at(i);
			}
		}
		catch (...)
		{
			throw IsChildren();
		}
		throw IsChildren();
	}
	else
	{
		for (size_t i = 0; i < args.size(); i++)
		{
			delete [] args.at(i);
		}
		InCGI *incgi = new InCGI(toCGI.getWriteEnd(), this->input_buffer, this->server);
		this->associatedInCgi = incgi;
		this->server.add(incgi, EPOLLOUT);

		OutCGI *outcgi = new OutCGI(fromCGI.getReadEnd(), this->server);
		this->associatedOutCgi = outcgi;
		this->server.add(outcgi, EPOLLIN);
	}
	return ;
}

void	InputSocket::updateCgiEnvp(std::vector<std::string>& vec_envp)
{
	std::string str;

	str = "GATEWAY_INTERFACE=CGI/1.1";
	vec_envp.push_back(str);

	str = "REQUEST_METHOD=";
	str += this->method;
	vec_envp.push_back(str);

	if (this->headers.count("content-length"))
	{
		str = "CONTENT_LENGTH=";
		str += this->headers["content-length"].at(0);
		vec_envp.push_back(str);
	}

	if (this->headers.count("content-type"))
	{
		str = "CONTENT-TYPE=";
		for (size_t i = 0; i < this->headers["content-type"].size(); i++)
		{
			if (i > 0)
				str += ", ";
			str += this->headers["content-type"].at(i);
		}
		vec_envp.push_back(str);
	}
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

