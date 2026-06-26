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

#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "status.hpp"
#include "typedef.hpp"
#include <cctype>
#include <cctype>
#include <map>
#include <sstream>
#include <string>

static int	no_version(int& state, InputSocket& inputSocket, int& status);
static int	getBodySize(size_t& bodySize, string_map& headers);
static int	check_headers(const string_map& headers);

void	InputSocket::process_headers(size_t& start)
{
	// std::cout << "in process header\n";

	if (this->version == "")
	{
		if (no_version(this->state, *this, this->status) == SUCCESS)
			(this->*process_functions[this->state])(start);
		return ;
	}

	while (start < this->inputBuffer.size())
	{
		if (fill_last_line(this->inputBuffer, this->lastLine, start, this->state) == STOP)
		{
			if (getBodySize(this->bodySize, this->headers) != SUCCESS)
				return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, this->connection));
			(this->*process_functions[this->state])(start);
			break;
		}

		if (check_last_line(this->lastLine) != SUCCESS)
		{
			return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, this->connection));
		}

		if (this->lastLine.size() > 0 && this->lastLine[this->lastLine.size() - 1] == '\n')
		{
			add_line_headers(this->lastLine, this->headers);
			if (check_headers(this->headers) != SUCCESS)
				return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, this->connection));
			this->lastLine.clear();
		}
	}
}

static int	no_version(int& state, InputSocket& inputSocket, int& status)
{
		if (inputSocket.getMethod() == "GET")
		{
			state = 6;
			return (SUCCESS);
		}
		else
		{
			setup_response(status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, inputSocket.getConnection());
			return (FAILURE);
		}
}

int	getBodySize(size_t& bodySize, string_map& headers)
{
	if (headers.count("content-length") && headers["content-length"].size() > 0)
	{
		std::stringstream ss;
		ss << headers["content-length"].at(0);
		ss >> bodySize;
		if (ss.fail())
			return (FAILURE);
		return (SUCCESS);
	}
	return (SUCCESS);
}

static int	check_headers(const string_map& headers)
{
	if (headers.count("host"))
		if (headers.at("host").size() > 1)
			return (FAILURE);
	return (SUCCESS);
}

