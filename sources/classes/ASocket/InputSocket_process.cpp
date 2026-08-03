/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:00:58 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 15:08:26 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cstdio>
#include <iostream>
#include <netinet/ip.h>
#include <cerrno>

void			updateInputBuffer(std::string& inputBuffer, int fd, int& status, Connection& connection);
static void		remove_trailing_carriage_return(std::string& inputBuffer, bool& endByBackslashR);

void InputSocket::process()
{
	this->connection->updateLastReceivedTime();
	#ifdef DEBUG
	std::cout << "in InputSocket process()\n";
	#endif
	if (this->status != SUCCESS)
		return ;
	updateInputBuffer(this->inputBuffer, this->fd, this->status, *this->connection);
	if (this->status != SUCCESS)
		return ;

	if (endByBackslashR == true)
	{
		size_t r_position = this->inputBuffer.find_first_not_of("\r");
		if (r_position != std::string::npos && this->inputBuffer[r_position] != '\n')
			return (setup_response(this->status, HTTPStatus::C_ERR + HTTPStatus::BAD_REQ, connection));
		this->endByBackslashR = false;
	}
	remove_trailing_carriage_return(this->inputBuffer, endByBackslashR);
	
	size_t	position = 0;
	(this->*process_functions[this->state])(position);
	if (this->fail())
		return ;
	if (position >= this->inputBuffer.size())
		this->inputBuffer.clear();
	#ifdef DEBUG
	std::cout << *this << "\n";
	#endif
}

static void	remove_trailing_carriage_return(std::string& inputBuffer, bool& endByBackslashR)
{
	size_t	position = inputBuffer.find_last_not_of("\r");
	if (position == std::string::npos)
	{
		inputBuffer.clear();
		endByBackslashR = true;
	}
	if (position + 1 != inputBuffer.size())
	{
		inputBuffer = std::string(inputBuffer, 0, position + 1);
		endByBackslashR = true;
	}
}

void	updateInputBuffer(std::string& inputBuffer, int fd, int& status, Connection& connection)
{
	if (inputBuffer == "")
	{
		char buf[BUFSIZ];
		ssize_t nb_read = recv(fd, buf, BUFSIZ, MSG_DONTWAIT | MSG_NOSIGNAL);
		if (nb_read < 0)
		{
			int errno_value = errno;
			logerror("recv", errno_value);
			status = FAILURE;
			return ;
		}
		else if (nb_read == 0)
		{
			status = TERMINATE; 
			return ;
		}
		else
		{
			connection.addMemoryUsage(nb_read);
			inputBuffer = std::string(buf, nb_read);
		}
	}
	else
	{
		#ifdef DEBUG
		std::cout << "-->ACTION: InputSocket does not read anything, buffer not empty\n";
		#endif
	}
}
