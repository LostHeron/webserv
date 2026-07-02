/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process_method.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:03:11 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:03:36 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "status.hpp"
#include "abnf.hpp"

static int	check_method(std::string& method);

void	InputSocket::process_method(size_t& pos)
{ 
	size_t space_pos = this->inputBuffer.find(' ', 0);
	if (space_pos == std::string::npos)
	{
		// no space found: add everything in the 'method' field
		this->method.append(this->inputBuffer);
		if (check_method(this->method) != SUCCESS)
			return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, this->connection));
		pos = this->inputBuffer.size();
		return ;
	}
	else
	{
		this->method.append(this->inputBuffer, pos, space_pos - pos);
		if (check_method(this->method) != SUCCESS)
			return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, this->connection));
		else
		{
			this->state++;
			pos = space_pos + 1;
			if (pos < this->inputBuffer.size())
				(this->*process_functions[this->state])(pos);
			return ;
		}
	}
}

static int	check_method(std::string& method)
{
	if (method == "" ||
		method.size() > INPUTSOCKET_MAX_SIZE ||
		method.find_first_not_of(ABNF_UPPER) != std::string::npos)
		return (FAILURE);
	return (SUCCESS);
}

