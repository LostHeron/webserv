/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToOutSocket.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:48:19 by jweber            #+#    #+#             */
/*   Updated: 2026/05/31 17:18:47 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ToOutSocket.hpp"
#include "error.hpp"
#include <fcntl.h>
#include <iostream>

ToOutSocket::ToOutSocket(int fd, bool& isLastBuffer, std::string& outputBuffer, Server &server):
	ASocket(server),
	isLastBuffer(isLastBuffer),
	outputBuffer(outputBuffer)
{
	this->fd = fd;//should we dup it ? i dont think so
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
}

ToOutSocket::~ToOutSocket()
{
}


void ToOutSocket::process()
{
	std::cout << "in ToOutSocket process\n";
	if (this->fd < 0)
		isLastBuffer = true;
	else
	{
		if (this->outputBuffer == "")
		{
			char buf[BUFSIZ];
			ssize_t nb_read = read(this->fd, buf, BUFSIZ);
			std::cout << "ToOutSocket read " << nb_read << " bytes\n";
			if (nb_read < 0)
			{
				logerror();
			}
			else if (nb_read == 0)
			{
				this->isLastBuffer = true;
			}
			else
			{
				this->outputBuffer = std::string(buf, nb_read);
				//std::cout << "buffer = '" << this->outputBuffer << "'\n";
			}
		}
	}
}
