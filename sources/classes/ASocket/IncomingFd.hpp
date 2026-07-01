/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncomingFd.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 18:03:56 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 18:04:12 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCOMINGFD_HPP
#define INCOMINGFD_HPP

#include "ASocket.hpp"
#include "Connection.hpp"
#include "error.hpp"
#include "status.hpp"
#include <cerrno>
#include <cstdio>
#include <queue>
#include <string>
#include <unistd.h>

class IncomingFd: public ASocket
{
	public:
		IncomingFd(Connection *connection);
		~IncomingFd();

		std::string	pop();

		void	process();

	private:
		IncomingFd();
		IncomingFd(const IncomingFd& other);
		const IncomingFd& operator=(const IncomingFd& other);

		std::queue<std::string> buffers;
};


std::string	IncomingFd::pop()
{
	std::string ret;
	if (this->buffers.size() > 0)
	{
		ret = this->buffers.front();
		this->buffers.pop();
	}
	return (ret);
}

void	IncomingFd::process()
{
	char buf[BUFSIZ];
	ssize_t nb_read = read(fd, buf, BUFSIZ);
	if (nb_read < 0)
	{
		int errno_value = errno;
		logerror("recv", errno_value);
		status = FAILURE;
		return ;
	}
	else if (nb_read == 0)
	{
		// other end closed the socket, no need to try and send data.
		status = FAILURE; 
		// rename this, it is not failure, but
		//	is used to make server clear ressources associated 
		//	with this InputSocket request and associated OutputSocket
		return ;
	}
	else
	{
		this->buffers.push(std::string(buf, nb_read));
	}
}

#endif
