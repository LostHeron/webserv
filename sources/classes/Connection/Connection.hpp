/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 13:54:54 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:28:10 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include "ASocket.hpp"
#include "InputSocket.hpp"
#include "OutputSocket.hpp"
#include "InCGI.hpp"
#include "OutCGI.hpp"
#include "OutCGI.hpp"
#include "OutputSocket.hpp"

class Connection
{
	public:
		Connection(int fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server);
		~Connection();

		InputSocket*	getInputSocket();
		OutputSocket*	getOutputSocket();
		InCGI*			getInCGI();
		OutCGI*			getOutCGI();

		void			add(ASocket*, int event);
		void			remove(ASocket*);

		uint8_t			*getPeerAddr();
		uint16_t		getPeerPort();
		uint16_t		getLocalPort();

		const HostList& getHostList() const;

		time_t			getStartTime() const;

	private:
		Connection();
		Connection(const Connection& other);
		const Connection& operator=(const Connection& other);

		// used to timeout request
		time_t			startTime;

		uint8_t			peerAddr[4];
		uint16_t		peerPort;
		uint16_t		localPort;

		InputSocket		inputSocket;
		OutputSocket	outputSocket;

		InCGI			*inCGI;
		OutCGI			*outCGI;

		Server&			server;
};

#endif
