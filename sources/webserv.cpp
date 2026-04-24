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

#include "RequestFactory.hpp"
#include <iostream>

int	main(int ac, char **av)
{
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
