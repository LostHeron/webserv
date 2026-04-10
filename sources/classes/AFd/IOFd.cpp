/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:32 by jweber            #+#    #+#             */
/*   Updated: 2026/04/10 17:02:29 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IOFd.hpp"
#include "AFd.hpp"
#include <unistd.h>

IOFd::IOFd(int fd, Server& server):
	AFd(server)
{
	this->fd = fd;
}

IOFd::~IOFd()
{
}

void IOFd::process()
{
	// ok and here should do stuff with the fd,
	// and read data and start parsing request
}
