/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 21:55:35 by jweber            #+#    #+#             */
/*   Updated: 2026/06/01 22:10:49 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

// so draft of this class:
// when a connection arrives,
// now the ListenSocket should create a
// Connection Structure.
// This Structure should have a InputSocket
// and an OutputSocket structure

#include "InCGI.hpp"
#include "InputSocket.hpp"
#include "OutCGI.hpp"
#include "OutputSocket.hpp"
#include "Server.hpp"
#include <netinet/in.h>
#include <stdint.h>

class Connection
{
	public:
		Connection(int fd, uint16_t localport, struct sockaddr_in& addr, Server& server);
		~Connection();

	private:
		Connection(const Connection& other);
		const Connection& operator=(const Connection& other);

		InputSocket		inputSocket;
		OutputSocket	outputSocket;

		InCGI			*inCGI;
		OutCGI			*outCGI;
};


Connection::Connection(int fd, uint16_t localport, struct sockaddr_in& addr, Server& server):
	inputSocket(fd, localport, addr, server),
	outputSocket(fd, server)
{
}


#endif // !CONNECTION_HPP
