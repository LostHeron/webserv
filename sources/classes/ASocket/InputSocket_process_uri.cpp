/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_uri.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:04:22 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:04:56 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "status.hpp"
#include "abnf.hpp"

static int	check_uri(std::string& uri);
static void	clear_uri(std::string& uri);

// goal: should fill and check URI
// - reject any uri containing invalid char, like newline etc.
// - should also remove .. and . for the URI before going to next step
// ?? do we treat url encoding and decoding ? like '/hi%20you.html' should
// be transformed to '/hi 20yo.html' that's some question we need to ask
void	InputSocket::process_uri(size_t& pos)
{
	// std::cout << "in process uri\n";
	size_t space_pos = this->input_buffer.find(' ', pos);
	size_t crlf = this->input_buffer.find("\r\n", pos);
	size_t lf = this->input_buffer.find("\n", pos);
	size_t delim = std::min(space_pos, std::min(crlf, lf));
	if (delim == std::string::npos)
	{
		this->uri.append(this->input_buffer, pos, this->input_buffer.size() - pos);
		if (check_uri(this->uri) != SUCCESS)
			return (setup_response(this->status, 400, *this, *static_cast<OutputSocket*>(this->associatedSocket)));
		pos = this->input_buffer.size();
	}
	else
	{
		if (delim > pos)
			this->uri.append(this->input_buffer, pos, delim - pos);
		if (check_uri(this->uri) || this->uri == "")
			return (setup_response(this->status, 400, *this, *static_cast<OutputSocket*>(this->associatedSocket)));
		clear_uri(this->uri);
		this->state++;
		pos = delim;
		if (pos < this->input_buffer.size())
			(this->*process_functions[this->state])(pos);
	}
	return ;
}

// what is an invalid uri ?
// if size is too large
// if it contains other than allowed characters
static int	check_uri(std::string& uri)
{
	if (uri.size() > INPUTSOCKET_MAX_SIZE ||
		uri.find_first_not_of(ABNF_PATH_ABEMPTY "[]{}<>?#") != std::string::npos
	)
		return (FAILURE);
	if (uri.size() > 0)
	{
		if (uri.at(0) != '/')
			return (FAILURE);
	}
	return (SUCCESS);
}

void split(std::vector<std::string>& res, std::string& src, std::string charset)
{
	size_t	start = 0;
	size_t	end = 0;

	while (start != std::string::npos && end != std::string::npos)
	{
		start = src.find_first_not_of(charset, end);
		end = src.find_first_of(charset, start);
		if (start == std::string::npos)
			return ;
		else if (end == std::string::npos)
		{
			res.push_back(std::string(src, start, src.size() - start));
			return ;
		}
		else
		{
			res.push_back(std::string(src, start, end - start));
		}
	}
}

// this function should clear uri, by removing '..'
// like '/hi/../you' into '/you'
static void	clear_uri(std::string& uri)
{
	std::vector<std::string>	splitted;
	bool						end_by_slash;
	std::vector<std::string>	transformed;

	end_by_slash = false;
	if (uri[uri.size() - 1] == '/')
		end_by_slash = true;

	split(splitted, uri, "/");
	for (size_t i = 0; i < splitted.size(); i++)
	{
		if (splitted[i] == ".")
			;
		else if (splitted[i] == ".." && transformed.size() > 0)
			transformed.pop_back();
		else if (splitted[i] == "..")
			;
		else
			transformed.push_back(splitted[i]);
	}
	uri = "";
	for (size_t i = 0; i < transformed.size(); i++)
	{
		uri += '/';
		uri += transformed[i];
	}
	if (transformed.size() == 0 || end_by_slash == true)
		uri += '/';
}

