/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 15:19:43 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sockets.hpp"
#include <iostream>
#include "unistd.h"

int	main(void)
{
	// some function to read info from config file
	// that would return a structure containing necessary information
	
	Server server;
	if (server.fail())
	{
		std::cerr << "could not launch server\n";
		return (1);
	}
	start(server);
	return (0);
}
