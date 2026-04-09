/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 13:51:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 13:51:40 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Listen.hpp"
#include "status.hpp"
#include <sys/epoll.h>

Server::Server():
	status(SUCCESS),
	epoll()
{
	if (this->epoll.fail())
	{
		this->status = FAILURE;
	}
	else
	{
		listens.push_back(Listen(0, 4343));
	}
	//listens.emplace_back(Args &&args...);
};

Server::~Server()
{
}

bool	Server::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}

void	Server::activate()
{
	if (this->fail() == true)
		return;
	for (size_t i = 0; i < listens.size(); i++)
	{
		listens[i].activate();
		if (listens[i].fail() == true)
		{
			this->status = FAILURE;
			return ;
		}
		//epoll_ctl(this->epoll.getFd(), EPOLL_CTL_ADD, listens[i].getFd(), struct epoll_event *event)
	}
}
