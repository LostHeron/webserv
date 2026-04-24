/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 15:41:30 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOFd.hpp"
#include "AFd.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>

IOFd::IOFd(int fd, Server& server):
	AFd(server)
{
	this->fd = fd;
}

IOFd::~IOFd()
{
	//close(this->fd);
}

void IOFd::process()
{
	char buf[BUFSIZ];
	ssize_t nb_read = read(this->fd, buf, BUFSIZ);
	if (nb_read < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "read: " << error_msg << "\n";
	}
	else if (nb_read == 0)
	{
		close(this->fd);
	}
	else
	{
		std::string str(buf, nb_read);
		std::cout << "received: '" << str << "' from server\n";
	}
}

bool	IOFd::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}
