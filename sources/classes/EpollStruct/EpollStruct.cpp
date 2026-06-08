/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollStruct.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:29:57 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 15:18:24 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"
#include "EpollStruct.hpp"
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include "status.hpp"
#include "error.hpp"
#include <unistd.h>
#include <iostream>
#include <cerrno>

EpollStruct::EpollStruct()
{
	this->status = SUCCESS;
	this->epfd = epoll_create(42);
	if (this->epfd < 0)
	{
		int	error_value = errno;
		logerror("epoll_create", error_value);
		this->status = FAILURE;
	}
	/*
	if (fcntl(this->epfd, F_SETFD, FD_CLOEXEC) < 0)
	{
		int	error_value = errno;
		logerror("epoll_create", error_value);
		this->status = FAILURE;
	}
	*/
}

EpollStruct::~EpollStruct()
{
	close(this->epfd);
}

bool	EpollStruct::fail()
{
	if (this->status == SUCCESS)
		return (false);
	else
		return (true);
}

int	EpollStruct::add(ASocket *abstract_socket, int event_flags)
{
	struct epoll_event	event;
	int					ret;

	event.data.ptr = abstract_socket;
	event.events = event_flags;
	ret = epoll_ctl(this->epfd, EPOLL_CTL_ADD, abstract_socket->getFd(), &event);
	if (ret < 0)
	{
		this->status = FAILURE;
		std::cerr << "COULD NOT ADD FD TO EPFD INSTANCE!\n";
		return (FAILURE);
	}
	return (SUCCESS);
}

void	EpollStruct::remove(ASocket *abstractSocket)
{
	struct epoll_event	event;
	int					ret;

	memset(&event, 0, sizeof(event));
	ret = epoll_ctl(this->epfd, EPOLL_CTL_DEL, abstractSocket->getFd(), &event);
	if (ret < 0)
		this->status = FAILURE;
}

int		EpollStruct::getFd()
{
	return (this->epfd);
}
