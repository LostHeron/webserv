/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_header.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:33:02 by jweber            #+#    #+#             */
/*   Updated: 2026/06/01 15:43:09 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "status.hpp"
#include "Connection.hpp"
#include <cctype>
#include <cctype>
#include <map>
#include <string>

static int	check_headers(const string_map& headers);
static void	no_version(InputSocket& inputSocket, int& status);

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

void	InputSocket::process_headers(size_t& start)
{
	// std::cout << "in process header\n";

	if (this->version == "")
	{
		return (no_version(*this, this->status));
	}

	// goal, fill in the map
	while (start < this->inputBuffer.size())
	{
		if (fill_last_line(this->inputBuffer, this->lastLine, start, this->state) == STOP)
		{
			(this->*process_functions[this->state])(start);
			break;
		}

		if (check_last_line(this->lastLine) != SUCCESS)
		{
			return (setup_response(this->status, 400, this->connection));
		}

		if (this->lastLine.size() > 0 && this->lastLine[this->lastLine.size() - 1] == '\n')
		{
			add_line_headers(this->lastLine, this->headers);
			if (check_headers(this->headers) != SUCCESS)
				return (setup_response(this->status, 400, this->connection));
			this->lastLine.clear();
		}
	}
}

static void	no_version(InputSocket& inputSocket, int& status)
{
		if (inputSocket.getMethod() == "GET")
		{
			// do stuff to stop parsing incoming data,
			// and process the request using only information
			// in 'method' and in 'uri'
			// return ...
		}
		else
		{
			return (setup_response(status, 400, inputSocket.getConnection()));
		}
		return ;
}

static int	check_headers(const string_map& headers)
{
	if (headers.count("host"))
		if (headers.at("host").size() > 1)
			return (FAILURE);
	return (SUCCESS);
}

