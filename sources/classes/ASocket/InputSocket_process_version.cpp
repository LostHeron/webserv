/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_version.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:05:59 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:06:49 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "status.hpp"
#include "abnf.hpp"
#include <cstdlib>
#include <cstring>

static int	check_version(const std::string& method, const std::string& version);

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
	size_t	crlf = this->input_buffer.find("\r\n", pos);
	size_t	lf = this->input_buffer.find("\n", pos);
	size_t	delim = std::min(crlf, lf);
	if (delim == std::string::npos)
	{
		this->version.append(this->input_buffer, pos, this->input_buffer.size() - pos);
		if (this->version.size() > INPUTSOCKET_MAX_SIZE)
			return (setup_response(this->status, 400, *static_cast<OutputSocket*>(this->associatedSocket)));
		pos = this->input_buffer.size();
	}
	else
	{
		size_t	until;
		if (this->input_buffer[delim] == '\r')
			until = delim + 2;
		else
			until = delim + 1;
		this->version.append(this->input_buffer, pos, delim - pos);

		size_t	trailing_space_pos = this->version.find_last_not_of(" ") + 1;
		this->version.erase(trailing_space_pos, version.size() - trailing_space_pos);

		if (check_version(this->method, this->version) != SUCCESS)
			return (setup_response(this->status, 400, *static_cast<OutputSocket*>(this->associatedSocket)));
		this->state++;
		pos = until;
		if (pos < this->input_buffer.size())
			(this->*process_functions[this->state])(pos);
	}
}

// should begin with 'HTTP/'
// and then two number separated by a '.'
// or should be empty with a 'GET' method
static int	check_version(const std::string& method, const std::string& version)
{
	if (version.size() > INPUTSOCKET_MAX_SIZE)
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
