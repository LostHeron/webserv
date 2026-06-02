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

#include "InputSocket.hpp"
#include "OutputSocket.hpp"
#include "status.hpp"
#include "abnf.hpp"

static int	check_method(std::string& method);

void	InputSocket::process_method(size_t& pos)
{ 
	size_t space_pos = this->input_buffer.find(' ', 0);
	if (space_pos == std::string::npos)
	{
		// no space found: add everything in the 'method' field
		this->method.append(this->input_buffer);
		if (check_method(this->method) != SUCCESS)
			return (setup_response(this->status, 400, *this, *static_cast<OutputSocket*>(this->associatedSocket)));
		pos = this->input_buffer.size();
		return ;
	}
	else
	{
		this->method.append(this->input_buffer, pos, space_pos - pos);
		if (check_method(this->method) != SUCCESS)
			return (setup_response(this->status, 400, *this, *static_cast<OutputSocket*>(this->associatedSocket)));
		else
		{
			this->state++;
			pos = space_pos + 1;
			if (pos < this->input_buffer.size())
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

