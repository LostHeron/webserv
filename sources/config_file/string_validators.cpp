/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_validators.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:21:58 by cviel             #+#    #+#             */
/*   Updated: 2026/06/22 14:02:12 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include "config_file.hpp"

static void	ip_validator(std::string const& ip_address);

void	path_validator(std::string const& path)
{
	if (path.empty() == true)
		throw std::invalid_argument("Path is empty");
	if (path[0] != '/')
		throw std::invalid_argument("Path must begin with '/'");
	for (std::string::const_iterator it = path.begin(); it != path.end(); ++it)
	{
		if (std::isprint(static_cast<unsigned char>(*it)) == 0)
			throw std::invalid_argument("Path must only contain printable characters");
	}
}

void	index_validator(std::string const& index)
{
	if (index.empty() == true)
		throw std::invalid_argument("Index is empty");
	for (std::string::const_iterator it = index.begin(); it != index.end(); ++it)
	{
		if (std::isprint(static_cast<unsigned char>(*it)) == 0 || *it == '/')
			throw std::invalid_argument("Path must only contain printable characters and no '/' characters");
	}
}

void	interface_validator(std::string const& interface)
{
	std::string::size_type	colon_pos = interface.find(':');

	if (colon_pos != std::string::npos)
	{
		ip_validator(interface.substr(0, colon_pos));
		ip_validator(interface.substr(colon_pos + 1));
		return ;
	}
	ip_validator(interface);
}

static void	ip_validator(std::string const& ip_address)
{
	std::stringstream	ip_stream(ip_address);

	if (ip_stream.fail())
		throw std::runtime_error("String stream failed to properly setup");

	for (unsigned int i = 0; i < 4; ++i)
	{
		unsigned char	c = ip_stream.peek();

		if (ip_stream.eof() || std::isdigit(c) == 0)
			throw std::invalid_argument("IP address is invalid");
		
		unsigned int	byte;

		ip_stream >> byte;
		if (byte >= 256)
			throw std::invalid_argument("IP address is invalid");

		if (i < 3)
		{
			c = ip_stream.get();
			if (ip_stream.eof() || c != '.')
				throw std::invalid_argument("IP address is invalid");
		}
	}
	ip_stream.peek();
	if (!ip_stream.eof())
		throw std::invalid_argument("IP address is invalid");
}

void	request_validator(std::string const& request)
{
	for (std::string::const_iterator it = request.begin(); it != request.end(); ++it)
	{
		if (std::isupper(static_cast<unsigned char>(*it) == 0))
			throw std::invalid_argument("Request is invalid (contains non alpha or lowercase characters)");
	}
}

void	non_empty_validator(std::string const& str)
{
	if (str.empty())
		throw std::invalid_argument("Argument is empty");
}
