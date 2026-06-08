/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:18:29 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:56:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection/Connection.hpp"
#include "IsChildren.hpp"
#include "sockets.hpp"
#include "ASocket.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cstring>
#include <exception>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <unistd.h>
#include <vector>

static void	timeout_connections(Server& server);

#define EVENT_SIZE 50

// run is a global variable set to 1 in the main files (webserv.cpp)
// it is set to 0 when using ^C to allow the program to quit
void	start(Server& server)
{
	struct epoll_event events[EVENT_SIZE];
	std::cout << "server is now running waiting for events\n";
	while (run != STOP)
	{
		int nb_events = epoll_wait(server.getEfd(), events, EVENT_SIZE, -1);
		#ifdef DEBUG
			sleep(1); // just to slow down server for debugging purposes
		#endif
		if (nb_events < 0)
		{
			logerror("epoll_wait", errno);
			usleep(1000); // is it necessary ?
		}
		else
		{
			// std::cout << nb_events << " event where received in the epoll_wait function\n";
			try
			{
				for (int i = 0; i < nb_events; i++)
				{
					ASocket* event = static_cast<ASocket*>(events[i].data.ptr);
					try
					{
						event->process();
						if (event->fail())
						{
							if (event->getConnection() != NULL)
								server.remove(event->getConnection());
							break;
						}
					}
					catch (IsChildren& e)
					{
						throw;
					}
					catch (std::exception& e)
					{
						std::cerr << e.what() << "\n";
					}
					catch (...)
					{
						std::cerr << "an error occured\n";
					}
					
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
				timeout_connections(server);
				(void) timeout_connections;
			}
			catch (IsChildren& e)
			{
				std::cout << e.what() << "\n";
				std::cout << "this catch is used in case of execve failure, "
					"so that the children can exit the program cleanly!\n"
					"it might not work with only that throw though, to be checked\n";
				break;
			}
		}
	}
}


static void	timeout_connections(Server& server)
{
	time_t	current_time = time(NULL);
	std::vector<Connection*>& connections = server.getConnections();
	for (size_t i = 0; i < connections.size(); i++)
	{
		if (current_time - connections[i]->getStartTime() > TTL_CONNECTION)
		{
			std::cerr << "CONNECTION GETTING TIMEDOUT!!!\n";
			server.remove(connections[i]);
		}
	}
}
