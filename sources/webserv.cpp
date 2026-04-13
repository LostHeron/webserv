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

	while (1)
	{
		// filling
		std::cout << "type: ";
		std::cin >> type;
		std::cout << std::endl << "header: ";
		std::cin >> header;
		std::cout << std::endl << "body: ";
		std::cin >> body;
		std::cout << std::endl;

		facto.setMessage(type + "/-/" + header + "/-/" + body + "\n");
		facto.setTarget("COCO");
		facto.setSender("JULES");
		facto.setType(type);
		facto.setHeader(header);
		facto.setBody(body);
		
		// building
		req = facto.createElement();


		// testing
		if (!req)
			continue;
		req->execute();
		delete req;
	}
	return (0);
}

