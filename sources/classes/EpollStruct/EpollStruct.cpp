/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollStruct.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:29:57 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 11:42:08 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EpollStruct.hpp"
#include <cstring>
#include <sys/epoll.h>
#include "status.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include <cerrno>

EpollStruct::EpollStruct()
{
	this->status = SUCCESS;
	this->fd = epoll_create(42);
	if (this->fd < 0)
	{
		std::string error_msg(strerror(errno));
		std::cerr << "epoll_create: " << error_msg << "\n";
		this->status = FAILURE;
	}
}

EpollStruct::~EpollStruct()
{
	close(this->fd);
}

bool	EpollStruct::fail()
{
	if (this->status == SUCCESS)
		return (false);
	else
		return (true);
}
