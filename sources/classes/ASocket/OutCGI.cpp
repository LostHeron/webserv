/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutCGI.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:21:38 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 14:31:23 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutCGI.hpp"
#include "status.hpp"
#include "error.hpp"
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

OutCGI::OutCGI(int fd, Server& server):
	ASocket(server)
{
	this->fd = dup(fd); // TODO DANGER, what happens if the DUP FAILS ?
	if (this->fd < 0)
	{
		// TODO DANGER, what happens if dup fails ?
		// throw an error ?
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0)
		this->status = FAILURE;
}

OutCGI::~OutCGI()
{
}

void OutCGI::process()
{
	if (cgi_out_buffer == "")
	{
		char buf[BUFSIZ];
		ssize_t nb_read = read(this->fd, buf, BUFSIZ);
		if (nb_read < 0)
		{
			logerror();
		}
		else
		{
			this->cgi_out_buffer = std::string(buf, nb_read);
			std::cout << "OutCgi read " << nb_read << " bytes\n";
			std::cout << "OutCgi buffer = '" << cgi_out_buffer << "'\n";
		}
	}
	return ;
}

