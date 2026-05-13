/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_validators.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:21:58 by cviel             #+#    #+#             */
/*   Updated: 2026/05/13 17:26:49 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include "config_file.hpp"

void	interface_validator(std::string const& interface)
{
	std::string::size_type	comma_pos = interface.find(':');

	if (comma_pos != std::string::npos)
	{
		ip_validator(interface.substr(0, comma_pos));
		ip_validator(interface.substr(comma_pos + 1));
		return ;
	}
	ip_validator(interface);
}

static void	ip_validator(std::string const& ip_address)
{
	std::stringstream	ip_stream(ip_address);

	if (ip_stream.fail())
		throw std::runtime_error("String stream failed to properly setup");

	for (unsigned int i = 0; i < IP_SIZE; ++i)
	{
		unsigned char	c = ip_stream.peek();

		if (ip_stream.eof() || std::isdigit(c) == 0)
			throw std::invalid_argument("IP address is invalid");
		
		unsigned int	byte;

		ip_stream >> byte;
		if (byte >= 256)
			throw std::invalid_argument("IP address is invalid");

		if (i < IP_SIZE - 1)
		{
			c = ip_stream.get();
			if (ip_stream.eof() || c != '.')
				throw std::invalid_argument("IP address is invalid");
		}
	}
	
	unsigned char	c = ip_stream.get();

	if (ip_stream.eof())
		return ;
	if (c != '/')
		throw std::invalid_argument("IP address is invalid");
	c = ip_stream.peek();
	if (ip_stream.eof() || std::isdigit(c) == 0)
		throw std::invalid_argument("IP address is invalid");
	
	unsigned int	mask;

	ip_stream >> mask;
	if (mask > IP_SIZE * 8)
		throw std::invalid_argument("IP address is invalid");
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

void	redirection_validator(std::string const& redir)
{
	std::string::size_type	first_occ = redir.find_first_of(':');
	
	if (first_occ == std::string::npos || redir.find_last_of(':') != first_occ)
		throw std::invalid_argument("Redirection is invalid or ambiguous (expected input : 'path1:path2')");
}

void	extension_validator(std::string const& extension)
{
	std::string::const_iterator	it = extension.begin();

	if (it == extension.end() || *it != '.' || ++it == extension.end())
		throw std::invalid_argument("Extension is empty or invalid");
	for (it; it != extension.end(); ++it)
	{
		if (std::isalpha(static_cast<unsigned char>(*it)) == 0)
			throw std::invalid_argument("Extension is invalid (contains non alpha characters)");
	}
}