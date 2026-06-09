/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 15:05:42 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "HTTPStatus.hpp"
#include "OutCGI.hpp"
#include "OutputSocket.hpp"
#include "IsChildren.hpp"
#include "Pipe.hpp"
#include "HeadersBuilder.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "status.hpp"
#include "error.hpp"
#include "Connection/Connection.hpp"
#include <cctype>
#include <cstddef>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
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

InputSocket::InputSocket(int fd, Connection* connection):
	ASocket(connection),
	state(0)
{
	this->fd = fd;
	if (fcntl(this->fd, F_SETFL, O_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
		//this->status = FAILURE;
	}
	if (fcntl(this->fd, F_SETFD, FD_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
		//this->status = FAILURE;
	}
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
	std::cout << "In InputSocket Destructor\n";
}

const std::string					&InputSocket::getMethod(void) const { return(this->method); }
const std::string					&InputSocket::getUri(void) const { return(this->uri); }
const std::string					&InputSocket::getVersion(void) const { return(this->version); }
const string_map					&InputSocket::getHeaders(void) const { return(this->headers); }
string_map							&InputSocket::getHeadersNoConst(void) { return(this->headers); }

void	updateInputBuffer(std::string& input_buffer, int fd, int& status);

void InputSocket::process()
{
	std::cout << "in InputSocket process()\n";
	if (this->status != SUCCESS)
		return ;
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
			int errno_value = errno;
			logerror("recv", errno_value);
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

void	setup_response(int& status, int errorCode, Connection* connection)
{
	connection->setVHost();
	Response resp(errorCode, *(connection->getVHost()) );
	status = FINISH;
	HeadersBuilder b;
	b.initialize()
	 .buildStatusLine("HTTP/1.1", errorCode)
	 .buildDate()
	 .buildCRLF()
	 .buildBody(resp.getContent());
	connection->getOutputSocket()->setup(resp.getResource().first, b.build());
}

void	InputSocket::process_body(size_t& pos)
{
	if (pos != 0)
		this->input_buffer = std::string(this->input_buffer, pos);
	return ;
}


void	InputSocket::prepareCGI(const std::string& script_name)
{
	char	*argv[2];
	char	str[] = "";
	argv[0] = str; 
	argv[1] = NULL;

	Pipe toCGI;
	Pipe fromCGI;

	int pid = fork();
	if (pid < 0)
	{
		int errno_value = errno;
		logerror("fork", errno_value);
		this->status = FAILURE;
		return ;
	}
	if (pid == 0)
	{
		//here is the child !
		try
		{
			this->connection->setIsChildren();
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
			this->updateCgiEnvp(vec_envp, script_name);

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

			execve(script_name.c_str(), argv, envp);
			int	errno_value = errno;
			logerror("execve", errno_value);
			for (size_t i = 0; i < formatted_envp.size(); i++)
			{
				delete [] formatted_envp.at(i);
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
		this->getConnection()->setCgiPid(pid);
		size_t body_size;
		char *end;
		if (this->headers.count("content-length") == 1) // something wrong ?
			body_size = std::strtol(this->headers["content-length"].at(0).c_str(), &end, 10);
		else
			body_size = 0;
		InCGI *incgi = new InCGI(toCGI.getWriteEnd(), body_size, this->input_buffer, this->connection);
		this->connection->add(incgi, EPOLLOUT);
		this->connection->setInCGI(incgi);

		OutCGI *outcgi = new OutCGI(fromCGI.getReadEnd(), this->connection);
		this->connection->add(outcgi, EPOLLIN);
		this->connection->setOutCGI(outcgi);
	}
	return ;
}

static std::string	get_IPv4_string_format(uint8_t addr[4]);
static std::string	get_port_string_format(uint16_t peer_port);

void	InputSocket::updateCgiEnvp(std::vector<std::string>& vec_envp, const std::string& script_name)
{
	std::string str;

	if (this->headers.count("content-length"))
	{
		str = "CONTENT_LENGTH=";
		str += this->headers["content-length"].at(0);
		this->headers.erase("content-length");
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
		this->headers.erase("content-type");
		vec_envp.push_back(str);
	}

	str = "GATEWAY_INTERFACE=CGI/1.1";
	vec_envp.push_back(str);

	// str = "PATH_INFO..."
	// not implemented yet, seems annoying to do
	// example : /cgi-bin/somescript/coucou
	// -> PATH_INFO = /coucou
	// must see when a ressource is an identified file on the server
	// and then set PATH_INFO to what is after, but it's annoying 
	// so not yet for now
	
	// str = "PATH_TRANSLATED..."
	// not implemented yet
	// anyway in the RFC 3875, it says that script
	// relying on the variable 'may suffer limited portabillity'
	// and it is implementation defined .. so let's see if we have
	// time to do it

	str = "QUERY_STRING=" + this->query_string;
	vec_envp.push_back(str);

	str = "REMOTE_ADDR=" + get_IPv4_string_format(this->connection->getPeerAddr());
	vec_envp.push_back(str);

	str = "REMOTE_PORT=" + get_port_string_format(this->connection->getPeerPort());
	vec_envp.push_back(str);

	str = "REQUEST_METHOD=" + this->method;
	vec_envp.push_back(str);

	str = "SCRIPT_NAME=" + this->uri;
	vec_envp.push_back(str);

	str = "SCRIPT_FILENAME=" + script_name;
	vec_envp.push_back(str);

	str = "SERVER_NAME=???"; // should retrived the vhost name
	vec_envp.push_back(str);

	str = "SERVER_PORT=" + get_port_string_format(this->connection->getLocalPort());
	vec_envp.push_back(str);

	str = "SERVER_PROTOCOLE=HTTP/1.1";
	vec_envp.push_back(str);

	str = "SERVER_SOFTWARE=ft_webserv";
	vec_envp.push_back(str);

	str = "REDIRECT_STATUS=200";
	vec_envp.push_back(str);

	for (string_map::const_iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		str = "HTTP_";
		str += it->first;
		for (std::string::iterator jt = str.begin(); jt != str.end(); jt++)
		{
			if (*jt == '-')
				*jt = '_';
			*jt = std::toupper(*jt);
		}
		str += "=";
		for (size_t i = 0; i < it->second.size(); i++)
		{
			if (i !=0)
				str +=", ";
			str += it->second.at(i);
		}
		vec_envp.push_back(str);
	}
}

static std::string	get_IPv4_string_format(uint8_t addr[4])
{
	std::stringstream ss;
	std::string res;
	for (int i = 0; i < 4; i++)
	{
		if (i != 0)
			res += ".";
		ss << static_cast<int>(addr[i]);
		std::string tmp;
		ss >> tmp;
		ss.clear();
		res += tmp;
	}
	return (res);
}

static std::string get_port_string_format(uint16_t peer_port)
{
	std::string res;
	std::stringstream ss;
	ss << peer_port;
	ss >> res;
	return (res);
}

void	InputSocket::process_skip_sp(size_t& pos)
{
	// std::cout << "in process skip spaces\n";
	size_t	non_sp_pos = this->input_buffer.find_first_not_of(" ", pos);
	if (non_sp_pos == this->input_buffer.npos)
	{
		pos = non_sp_pos;
		return ;
	}
	this->state++;
	pos = non_sp_pos;
	if (pos < this->input_buffer.size())
		(this->*process_functions[this->state])(pos);
	return ;
}

std::ostream& operator<<(std::ostream& os, const InputSocket& inputSocket)
{
	os << "connection: ";
	for (int i = 0; i < 4; i++)
	{
		os << static_cast<int>(inputSocket.connection->getPeerAddr()[i]);
		if (i != 3)
			os << ".";
	}
	os << ":" << inputSocket.connection->getPeerPort() << ";\n";
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
	return (os);
}

