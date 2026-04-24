/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:54:41 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 13:51:48 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestFactory.hpp"
#include <iostream>

int	main(void)
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
		std::cout << std::endl << "sender: ";
		std::cin >> sender;
		std::cout << std::endl;
		std::cout << "type: ";
		std::cin >> type;
		std::cout << std::endl << "header: ";
		std::cin >> header;
		std::cout << std::endl << "body: ";
		std::cin >> body;
		std::cout << std::endl;

		// filling
		facto.setMessage(type + "/-/" + header + "/-/" + body + "\n");
		facto.setTarget(target);
		facto.setSender(sender);
		facto.setType(type);
		facto.setHeader(header);
		facto.setBody(body);
		
		// building
		req = facto.createElement();


		// testing
		if (!req)
			continue;

		// output
		req->execute();
		delete req;
	}
	return (0);
}

