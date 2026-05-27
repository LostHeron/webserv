/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/05/19 16:39:58 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_file.hpp"
#include "HostList.hpp"
#include "Server.hpp"
#include "RequestFactory.hpp"
#include "sockets.hpp"
#include "signals_handling.hpp"
#include "status.hpp"
#include <iostream>

int run = 1;

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	// some function to read info from config file
	// that would return a structure containing necessary information
	
	if (setup_signals() != SUCCESS)
	{
		std::cerr << "could not setup signals\n";
		return (1);
	}
	try
	{	
		HostList	host_list(HostList::build(av[1]));	
	}
	catch(const std::exception& e)
	{
		std::cerr << "Config file error: " << e.what() << std::endl;
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
