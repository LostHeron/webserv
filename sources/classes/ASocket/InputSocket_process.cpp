/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket_process.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 09:00:58 by jweber            #+#    #+#             */
/*   Updated: 2026/06/26 10:03:03 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InputSocket.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cstdio>
#include <iostream>
#include <netinet/ip.h>
#include <cerrno>

void	updateInputBuffer(std::string& inputBuffer, int fd, int& status);

void InputSocket::process()
{
	#ifdef DEBUG
	std::cout << "in InputSocket process()\n";
	#endif
	if (this->status != SUCCESS)
		return ;
	updateInputBuffer(this->inputBuffer, this->fd, this->status);
	if (this->status != SUCCESS)
		return ;

	size_t	position = 0;
	std::cout << "this->state = " << this->state << "\n";
	(this->*process_functions[this->state])(position);
	if (this->fail())
		return ;
	if (position >= this->inputBuffer.size())
		this->inputBuffer.clear();
	#ifdef DEBUG
	std::cout << *this << "\n";
	#endif
}

void	updateInputBuffer(std::string& inputBuffer, int fd, int& status)
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
			status = FAILURE; 
			// rename this, it is not failure, but
			//	is used to make server clear ressources associated 
			//	with this InputSocket request and associated OutputSocket
			return ;
		}
		else
		{
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
