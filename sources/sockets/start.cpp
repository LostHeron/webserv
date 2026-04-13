/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:18:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 15:35:00 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "AFd.hpp"
#include <cstring>
#include <sys/epoll.h>
#include <string>
#include <cerrno>
#include <iostream>
#include <unistd.h>

#define EVENT_SIZE 50

void	start(Server& server)
{
	(void) server;
	while (true)
	{
		struct epoll_event events[EVENT_SIZE];
		int nb_events = epoll_wait(server.getEfd(), events, EVENT_SIZE, -1);
		if (nb_events < 0)
		{
			std::string error_msg(strerror(errno));
			std::cerr << "epoll_wait: " << error_msg << "\n";
			usleep(1000); // is it necessary ?
		}
		else
		{
			std::cout << nb_events << " event where received in the epoll_wait function\n";
			for (int i = 0; i < nb_events; i++)
			{
				AFd* ptr = static_cast<AFd*>(events[i].data.ptr);
				ptr->process();
			}
		}
	}
}
