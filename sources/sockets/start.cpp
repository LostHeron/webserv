/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:18:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:39:04 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets.hpp"
#include "AFd.hpp"
#include "status.hpp"
#include <cstring>
#include <sys/epoll.h>
#include <string>
#include <cerrno>
#include <iostream>
#include <unistd.h>

#define EVENT_SIZE 50

// run is a global variable set to 1 in the main files (webserv.cpp)
// it is set to 0 when using ^C to allow the program to quit
void	start(Server& server)
{
	struct epoll_event events[EVENT_SIZE];
	while (run != 0)
	{
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
				AFd* event = static_cast<AFd*>(events[i].data.ptr);
				event->process();
				if (event->fail())
					server.remove(event);
				// here someking of code like :
				/* try
				 * {
				 *		event->process
				 * }
				 * catch (std::exception& e)
				 * {
				 *		server.free_space() // (some function that would \
				 *		search for the more consumming process, terminate the  \
				 *		connection and removed all ressources associated with the process) \
				 * }
				*/
			}
		}
	}
}
