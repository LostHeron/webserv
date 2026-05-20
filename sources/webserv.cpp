/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/05/19 16:39:58 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_file.hpp"
#include "HostList.hpp"
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
		HostList	host_list(HostList::build(av[1]));	
	}
	catch(const std::exception& e)
	{
		std::cerr << "Config file error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
	Server server;
	if (server.fail())
	{
		std::cerr << "could not launch server\n";
		return (1);
	}
	start(server);
	return (0);
}
