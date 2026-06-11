/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:01:33 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:56:10 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"
#include "Server.hpp"
#include "status.hpp"
#include "Connection.hpp"
#include <unistd.h>

ASocket::ASocket(Connection* connection):
	fd(-1),
	status(SUCCESS),
	connection(connection)
{
}

ASocket::~ASocket()
{
	if (this->fd >= 0)
		close(this->fd);
	this->fd = -1;
}

int	ASocket::getFd() const
{
	return (this->fd);
}

Connection		*ASocket::getConnection()
{
	return (this->connection);
}

bool	ASocket::fail()
{
	if (this->status == FAILURE)
		return (true);
	else
		return (false);
}

bool	ASocket::terminate()
{
	if (this->status == TERMINATE)
		return (true);
	else
		return (false);
}

int fill_last_line(const std::string &buf, std::string &last_line, size_t &start, int &state)
{
	static std::vector<std::string> delims;
	if (delims.size() == 0)
	{
		delims.reserve(2);
		delims.push_back("\n");
		delims.push_back("\r\n");
	}
	size_t	delimPosition;
	size_t	until;

	delimPosition = getDelimPosition(buf, start, delims);
	if (delimPosition == std::string::npos) // no \r\n
	{
		until = buf.size();
		last_line.append(buf, start, until - start);
	}
	else 
	{
		if (buf[delimPosition] == '\r')
			until = delimPosition + 2;
		else
			until = delimPosition + 1;

		if (delimPosition == start && last_line.size() == 0)
		{
			state++;
			start = until;
			return (STOP);
		}
		else
		{
			last_line.append(buf, start, until - start);
		}
	}
	start = until;
	return (CONTINUE);
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

# include "abnf.hpp"
# define ALLOWED_CHAR_KEY   ABNF_ALPHA ABNF_DIGIT ":_;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%"
# define ALLOWED_CHAR_VALUE ALLOWED_CHAR_KEY " "

// "coucou\r\n" -> valid
// coucou\r:\r\n -> invalid
// coucou:yo\ryo\r\n -> invalid
int check_last_line(std::string last_line)
{
	size_t	colonPosition = last_line.find(":");

	remove_trailing_new_line(last_line);
	if (colonPosition == std::string::npos)
	{
		if (last_line.find_first_not_of(ALLOWED_CHAR_KEY) != std::string::npos)
			return (FAILURE);
	}
	else
	{
		size_t	value_begin = colonPosition + 1;

		std::string	key(last_line, 0, colonPosition);
		if (key.find_first_not_of(ALLOWED_CHAR_KEY) != std::string::npos)
			return (FAILURE);
		if (value_begin < last_line.size()) // for line that look like : 'host:' to avoid checking forbidden character in non existing value
		{
			std::string	value(last_line, value_begin, last_line.size() - value_begin);
			if (value.find_first_not_of(ALLOWED_CHAR_VALUE) != std::string::npos)
				return (FAILURE);
		}
	}
	return (SUCCESS);
}

void	remove_trailing_new_line(std::string& line)
{
	if (line.size() > 0 && (line[line.size() - 1] == '\n' || line[line.size() - 1] == '\r'))
	{
		line.erase(line.size() - 1, 1);
		while (line.size() > 0 && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1, 1);
	}
}

static void	lowering(std::string& line);

void add_line_headers(std::string& line, string_map& headers)
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
