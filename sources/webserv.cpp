/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:20:32 by jweber            #+#    #+#             */
/*   Updated: 2026/05/04 15:26:01 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sockets.hpp"
#include "signals_handling.hpp"
#include "RequestFactory.hpp"
#include "status.hpp"
#include <iostream>

int run = 1;

int	main(int ac, char **av)
{
	(void) ac; (void)av;
	RequestFactory facto;

	std::string message;
	std::string target;
	std::string sender;

	std::string type;
	std::string header;
	std::string body;

	ARequest *req;

	while (std::cin)
	{
		// input
		std::cout << "target: ";
		std::cin >> target;
		std::cout << "sender: ";
		std::cin >> sender;
		std::cout << "type: ";
		std::cin >> type;
		std::cout << "header: ";
		std::cin >> header;
		std::cout << "body: ";
		std::cin >> body;
		std::cout << std::endl;

		// filling
		facto.setMessage("\n------------------\n" + header + "\n" + body + "\n------------\n");
		facto.setTarget(target);
		facto.setSender(sender);
		facto.setType(type);
		facto.setHeader(header);
		facto.setBody(body);
		
		// building
		req = facto.createElement();


		// testing
		if (!req)
		{
			std::cout << "ERROR!" << std::endl;
			continue;
		}

		// output
		std::cout << "from: " << req->getSender() << " to: " << req->getTarget() << "\nmsg: " << req->getMessage() << std::endl;
		std::cout << "type: " << req->getType() << "\nheader: " << req->getHeader() << " body: " << req->getBody() << std::endl;
		req->execute();
		delete req;
	}
	return (0);
}
