/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 16:38:43 by jweber            #+#    #+#             */
/*   Updated: 2026/06/11 16:38:43 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_file.hpp"
#include "VHostList.hpp"
#include "Server.hpp"
#include "RequestFactory.hpp"
#include "sockets.hpp"
#include "signals_handling.hpp"
#include "status.hpp"
#include <cstdio>
#include <iostream>
#include <sys/resource.h>

int run = 1;

int	main(int ac, char **av)
{
	// some function to read info from config file
	// that would return a structure containing necessary information
	
	#ifdef INFO_WEBSERV__
	struct rlimit r;
	if (getrlimit(RLIMIT_AS, &r) < 0)
		return (perror("getrlimit"), 1);
	r.rlim_cur = 50000000;
	if (setrlimit(RLIMIT_AS, &r) < 0)
		return (perror("getrlimit"), 1);
	#endif
	
	if (setup_signals() != SUCCESS)
	{
		std::cerr << "could not setup signals\n";
		return (1);
	}

	if (ac > 2)
	{
		std::cerr << "usage: ./webserv [config_file]\n";
		return (1);
	}
	try
	{	
		//HostList	host_list(HostList::build(av[1]));	

		char *filename;
		if (ac == 1)
			filename = (char *)"webserv.conf";
		else
			filename = av[1];
		Server server(filename);
		if (server.fail())
		{
			std::cerr << "could not launch server\n";
			return (1);
		}

		start(server);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Config file error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}



set-cookies:	1=bonjour
set-cookies:	2=23


cookies:		1=yo; 2=salut


if (client.jsonObj.goodClient() && client.jsonObj.hasChanged(this->_cookies))
	client.jsonObj.update()
else
	client.jsonObj.create(this->_cookies)


