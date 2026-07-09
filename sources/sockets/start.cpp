/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:18:29 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 14:51:03 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "HTTPStatus.hpp"
#include "InputSocket.hpp"
#include "IsChildren.hpp"
#include "Server.hpp"
#include "sockets.hpp"
#include "ASocket.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cstring>
#include <exception>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <sys/socket.h>
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
		static int a;
		if (a < 100)
			usleep(10000);
		a++;
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
						if (event->fail() || event->terminate())
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
						if (event->getConnection() != NULL)
							server.remove(event->getConnection());
						break;
					}
					catch (...)
					{
						std::cerr << "an error occured\n";
						if (event->getConnection() != NULL)
							server.remove(event->getConnection());
						break;
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
				std::vector<Connection *>& connections = server.getConnections();
				for (size_t i = 0; i < connections.size(); i++)
				{
					if (connections[i]->isChunked() == true)
					{
						if (connections[i]->getChunk().isFinished() == true &&
							connections[i]->getOutCGI() == NULL)
						{
							connections[i]->getInputSocket()->launch_cgi(connections[i]->getChunk().getTotalSize());
						}
					}
				}
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
			int	a;
			if (connections[i]->isCGI() == true)
			{
				setup_response(a, HTTPStatus::S_ERR + HTTPStatus::BAD_GATEWAY, connections[i]);
			}
			else
			{
				setup_response(a, HTTPStatus::C_ERR + HTTPStatus::TIMEOUT, connections[i]);
			}
		}
	}
}
