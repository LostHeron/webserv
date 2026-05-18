/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/05/13 17:50:10 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualHost.hpp"
#include "config_file.hpp"
#include "Server.hpp"
#include "sockets.hpp"
#include <iostream>

int	main(int ac, char **av)
{
	// some function to read info from config file
	// that would return a structure containing necessary information
	
	if (ac != 2)
	{
		std::cerr << "You must specify a config file only" << std::endl;
		return (1);
	}
	
	try
	{	
		std::map<u_int16_t, VirtualHost>	host_map;
		
		host_setup(av[1], host_map);
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
