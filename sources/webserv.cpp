/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 18:42:57 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sockets.hpp"
#include <iostream>

void    host_setup(char const* filename);

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
		host_setup(av[1]);
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
