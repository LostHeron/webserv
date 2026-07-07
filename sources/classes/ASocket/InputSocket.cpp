/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 15:05:06 by jweber           ###   ########.fr       */
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
#include "Connection.hpp"
#include "Environment.hpp"
#include "Response.hpp"
#include "PUTReq.hpp"
#include <fcntl.h>
#include <sstream>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
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
	state(0),
	endByBackslashR(false),
	bodySize(0),
	nbSent(0),
	resp(NULL),
	req(NULL)
{
	this->fd = dup(fd);
	if (this->fd < 0)
		setup_response(this->status, HTTPStatus::S_ERR, connection);
	if (fcntl(this->fd, F_SETFL, O_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
		setup_response(this->status, HTTPStatus::S_ERR, connection);
	}
	if (fcntl(this->fd, F_SETFD, FD_CLOEXEC) < 0)
	{
		int error_value = errno;
		logerror("fcntl", error_value);
		setup_response(this->status, HTTPStatus::S_ERR, connection);
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
	#ifdef DEBUG
	std::cout << "In InputSocket Destructor\n";
	#endif
	if (this->resp != NULL)
	{
		delete (this->resp);
		this->resp = NULL;
	}
	if (this->req != NULL)
	{
		delete (this->req);
		this->req = NULL;
	}
}

const std::string					&InputSocket::getMethod(void) const { return(this->method); }
const std::string					&InputSocket::getUri(void) const { return(this->uri); }
const std::string					&InputSocket::getVersion(void) const { return(this->version); }
const string_map					&InputSocket::getHeaders(void) const { return(this->headers); }
string_map							&InputSocket::getHeadersNoConst(void) { return(this->headers); }


void	setup_response(int& status, int errorCode, Connection* connection)
{
	connection->setVHost();
	Response resp(errorCode, *(connection->getVHost()) );
	status = FINISH;
	HeadersBuilder b;
	b.initialize()
	 .buildStatusLine("HTTP/1.0", errorCode)
	 .buildDate()
	 .buildCRLF()
	 .buildBody(resp.getContent());
	connection->getOutputSocket()->setup(resp.getResource().first, b.build());
}

void	InputSocket::process_body(size_t& pos)
{
	if (pos != 0)
	{
		this->inputBuffer = std::string(this->inputBuffer, pos);
		pos = 0; // to avoid InputSocket::process clear the string
	}
	if (this->connection->isChunked() == true)
	{
		this->connection->getChunk().process(this->inputBuffer);
		if (this->connection->getChunk().getStatus() == SUCCESS)
		{
			return (setup_response(this->status, this->connection->getChunk().getStatus(), connection));
		}
	}
	else
	{
		if (this->req != NULL)
		{
			/*
			 * TODO handle PUT request in case of transfer encoding ?
			 */
			if (nbSent + this->inputBuffer.size() > this->bodySize)
				this->inputBuffer = std::string(this->inputBuffer, 0, this->bodySize - nbSent);
			unsigned ret = dynamic_cast<PUTReq*>(this->req)->appendBodyToFile(this->inputBuffer);
			if (ret != SUCCESS)
				setup_response(this->status, ret, connection);
			nbSent += this->inputBuffer.size();
			this->inputBuffer.clear();
			if (nbSent == this->bodySize)
				setup_response(this->status, resp->getStatus(), this->connection);
			// means this is a put request, and
			// here, i need a function to send data to the post ?
		}
	}
	return ;
}


void launch_child_process(InputSocket &inputSocket, Response& resp, Pipe& toCGI, Pipe& fromCGI);

void	InputSocket::launch_cgi(size_t nbToSend)
{

	Pipe toCGI;
	Pipe fromCGI;

	// without those, buffer might be not empty
	// and end up in the buffer of the child,
	// or at least, it's what seemed to be
	std::cout << std::endl;
	std::cerr << std::endl;

	int pid = fork();
	if (pid < 0)
	{
		int errno_value = errno;
		logerror("fork", errno_value);
		int a;
		return (setup_response(a, HTTPStatus::S_ERR + HTTPStatus::INTERNAL, this->connection));
	}
	if (pid == 0)
	{
		launch_child_process(*this, *this->resp, toCGI, fromCGI);
	}
	else
	{
		this->getConnection()->setCgiPid(pid);
		if (nbToSend != 0)
		{
			InCGI *incgi = new InCGI(toCGI.getWriteEnd(), nbToSend, this->inputBuffer, this->connection);
			this->connection->add(incgi, EPOLLOUT);
			this->connection->setInCGI(incgi);
		}

		OutCGI *outcgi = new OutCGI(fromCGI.getReadEnd(), this->connection);
		this->connection->add(outcgi, EPOLLIN);
		this->connection->setOutCGI(outcgi);
	}
	return ;
}

void	setup_child_standard_io_fds(Pipe& toCGI, Pipe& fromCGI);

void launch_child_process(InputSocket &inputSocket, Response& resp, Pipe& toCGI, Pipe& fromCGI)
{
	char	*argv[2];
	char	str[] = "";
	argv[0] = str; 
	argv[1] = NULL;

	const std::string& script_name = resp.getResource().second;
	const std::string& pathInfo = resp.getPathInfo();

	try
	{
		inputSocket.getConnection()->setIsChildren();

		size_t	last_slash_pos = script_name.rfind("/");
		std::string new_dir = std::string(script_name, 0, last_slash_pos);
		if (chdir(new_dir.c_str()) < 0)
			throw IsChildren();

		setup_child_standard_io_fds(toCGI, fromCGI);

		Environment env(script_name, pathInfo, inputSocket);

		#ifdef DEBUG
		std::cerr << "exceve will execute : " << script_name << "\n";
		#endif
		execve(script_name.c_str(), argv, env.getEnvp());
		int	errno_value = errno;
		logerror("execve", errno_value);
	}
	catch (...)
	{
		throw IsChildren();
	}
	throw IsChildren();
}

void	setup_child_standard_io_fds(Pipe& toCGI, Pipe& fromCGI)
{
	toCGI.closeWriteEnd();
	fromCGI.closeReadEnd();
	if (dup2(toCGI.getReadEnd(), STDIN_FILENO) < 0)
	{
		throw IsChildren();
	}
	if (dup2(fromCGI.getWriteEnd(), STDOUT_FILENO) < 0)
	{
		throw IsChildren();
	}
	fromCGI.closeWriteEnd();
	toCGI.closeReadEnd();
	return ;
}

static std::string	get_IPv4_string_format(uint8_t addr[4]);
static std::string	get_port_string_format(uint16_t peer_port);

void	InputSocket::updateCgiEnvp(std::vector<std::string>& vec_envp, const std::string& script_name, const std::string& pathInfo)
{
	std::string str;

	if (this->connection->isChunked() == true)
	{
		str = "CONTENT_LENGTH=";
		std::stringstream ss;
		ss << this->connection->getChunk().getTotalSize();
		std::string tmp;
		ss >> tmp;
		str += tmp;
	}
	else if (this->headers.count("content-length"))
	{
		str = "CONTENT_LENGTH=";
		str += this->headers["content-length"].at(0);
		this->headers.erase("content-length");
		vec_envp.push_back(str);
	}

	if (this->headers.count("content-type"))
	{
		str = "CONTENT_TYPE=";
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

	str = "PATH_INFO=/" + pathInfo;
	vec_envp.push_back(str);
	
	// str = "PATH_TRANSLATED..."
	// not implemented yet
	// anyway in the RFC 3875, it says that script
	// relying on the variable 'may suffer limited portabillity'
	// and it is implementation defined .. so let's see if we have
	// time to do it

	str = "QUERY_STRING=" + this->queryString;
	vec_envp.push_back(str);

	str = "REMOTE_ADDR=" + get_IPv4_string_format(this->connection->getPeerAddr());
	vec_envp.push_back(str);

	str = "REMOTE_PORT=" + get_port_string_format(this->connection->getPeerPort());
	vec_envp.push_back(str);

	str = "REQUEST_METHOD=" + this->method;
	vec_envp.push_back(str);

	/*
	// commented because it caused cgi_tester to
	// return PATH_INFO incorrect when it was set
	str = "SCRIPT_NAME=" + this->uri;
	vec_envp.push_back(str);
	*/

	str = "SCRIPT_FILENAME=" + script_name;
	vec_envp.push_back(str);

	str = "SERVER_NAME=???"; // should retrived the vhost name
	vec_envp.push_back(str);

	str = "SERVER_PORT=" + get_port_string_format(this->connection->getLocalPort());
	vec_envp.push_back(str);

	str = "SERVER_PROTOCOL=HTTP/1.1";
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
	size_t	non_sp_pos = this->inputBuffer.find_first_not_of(" ", pos);
	if (non_sp_pos == this->inputBuffer.npos)
	{
		pos = non_sp_pos;
		return ;
	}
	this->state++;
	pos = non_sp_pos;
	if (pos < this->inputBuffer.size())
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
	os << "current buffer contains: '" << inputSocket.inputBuffer << "'\n";
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

