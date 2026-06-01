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
#include "abnf.hpp"
#include "status.hpp"
#include <cctype>
#include <cctype>
#include <map>
#include <string>
#include <vector>

static int	check_headers(const string_map& headers);
static void	no_version(const InputSocket& inputSocket, int& status);
static void add_line_headers(std::string& line,  string_map& headers);
static void	lowering(std::string& line);

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
	while (start < this->input_buffer.size())
	{
		if (fill_last_line(this->input_buffer, this->last_line, start, this->state) == STOP)
		{
			(this->*process_functions[this->state])(start);
			break;
		}

		if (check_last_line(this->last_line) != SUCCESS)
		{
			return (send_bad_request(this->fd, this->status));
		}

		if (this->last_line.size() > 0 && this->last_line[last_line.size() - 1] == '\n')
		{
			add_line_headers(this->last_line, this->headers);
			if (check_headers(this->headers) != SUCCESS)
				return (send_bad_request(this->fd, this->status));
			this->last_line.clear();
		}
	}
}

static void	no_version(const InputSocket& inputSocket, int& status)
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
			return (send_bad_request(inputSocket.getFd(), status));
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

static void add_line_headers(std::string& line, string_map& headers)
{
	size_t		colonPosition = line.find(":");
	std::string	key;

	remove_trailing_new_line(line);
	if (colonPosition == std::string::npos)
	{
		lowering(line);
		headers[line].push_back("");
	}
	else
	{
		size_t	value_begin = line.find_first_not_of(" ", colonPosition + 1);
		size_t	value_end = line.find_last_not_of(" ");
		key.append(line, 0, colonPosition);
		lowering(key);
		if (value_end == std::string::npos || value_begin == std::string::npos)
			headers[key].push_back("");
		else
			headers[key].push_back(std::string(line, value_begin, value_end - value_begin + 1));
	}
}

static void	lowering(std::string& line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		line[i] = std::tolower(line[i]);
	}
}
