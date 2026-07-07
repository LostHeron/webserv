/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_version.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:05:59 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 15:04:22 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "status.hpp"
#include "abnf.hpp"
#include <cstdlib>
#include <cstring>
#include "Connection.hpp"

static int	check_version(const std::string& method, std::string& version);

// here function to process version part of the request
// should look something like: HTTP/*[DIGITS].*[DIGITS] or nothing 
// with a 'GET' method
// condition of failed version:
//	- version field greater than 2048 char (arbitrary size)
//	- version number not supported ?
//	- 
void	InputSocket::process_version(size_t& pos)
{
	// std::cout << "in process version\n";
	size_t	crlf = this->inputBuffer.find("\r\n", pos);
	size_t	lf = this->inputBuffer.find("\n", pos);
	size_t	delim = std::min(crlf, lf);
	if (delim == std::string::npos)
	{
		this->version.append(this->inputBuffer, pos, this->inputBuffer.size() - pos);
		if (this->version.size() > INPUTSOCKET_MAX_SIZE)
			return (setup_response(this->status, 400, this->connection));
		pos = this->inputBuffer.size();
	}
	else
	{
		size_t	until;
		if (this->inputBuffer[delim] == '\r')
			until = delim + 2;
		else
			until = delim + 1;
		this->version.append(this->inputBuffer, pos, delim - pos);

		size_t	trailing_space_pos = this->version.find_last_not_of(" ") + 1;
		this->version.erase(trailing_space_pos, version.size() - trailing_space_pos);

		int ret = check_version(this->method, this->version) ;
		if (ret != SUCCESS)
			return (setup_response(this->status, ret, this->connection));
		this->state++;
		pos = until;
		if (pos < this->inputBuffer.size())
			(this->*process_functions[this->state])(pos);
	}
}

// should begin with 'HTTP/'
// and then two number separated by a '.'
// or should be empty with a 'GET' method
static int	check_version(const std::string& method, std::string& version)
{
	if (version.size() > INPUTSOCKET_MAX_SIZE)
		return (HTTPStatus::C_ERR + HTTPStatus::BAD_REQ);

	if (version == "")
	{
		if (method != "GET")
			return (HTTPStatus::C_ERR + HTTPStatus::BAD_REQ);
		return (SUCCESS);
	}

	if (std::strncmp(version.c_str(), "HTTP/", 5) != 0)
		return (HTTPStatus::C_ERR + HTTPStatus::BAD_REQ);

	size_t	dot_pos = version.find_first_not_of(ABNF_DIGIT, 5);
	if (dot_pos == std::string::npos || version.at(dot_pos) != '.')
		return (HTTPStatus::C_ERR + HTTPStatus::BAD_REQ);

	long major = std::strtol(version.c_str() + 5, NULL, 10);
	if (major != 1)
		return (HTTPStatus::S_ERR + HTTPStatus::HTTP_VERSION_NOT_SUPPORTED);

	size_t	end_pos = version.find_first_not_of(ABNF_DIGIT, dot_pos + 1);
	if (end_pos != std::string::npos)
		return (HTTPStatus::C_ERR + HTTPStatus::BAD_REQ);

	/*
	// this check is no longer needed, if we support HTTP/1.0,
	// all other HTTP/1.0+ will be supported by us, certain functionnality
	// will not be implemented, but the request should still be fine to process
	long minor = std::strtol(version.c_str() + dot_pos, NULL, 10);
	if (minor != 0 && minor != 1 && minor != 2 && minor != 3)
		return (HTTPStatus::S_ERR + HTTPStatus::HTTP_VERSION_NOT_SUPPORTED);
	*/

	return (SUCCESS);
}
