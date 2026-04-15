/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 10:31:20 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sockets.hpp"
#include "signals_handling.hpp"
#include "status.hpp"
#include <iostream>

int run = 1;

int	main(void)
{
	// some function to read info from config file
	// that would return a structure containing necessary information
	
	if (setup_signals() != SUCCESS)
	{
		std::cerr << "could not setup signals\n";
		return (1);
	}

	Server server;
	if (server.fail())
	{
		std::cerr << "could not launch server\n";
		return (1);
	}

	start(server);
	return (0);
}
