/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 13:54:54 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 18:48:30 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include "ASocket.hpp"
#include "RequestMetaData/RequestMetaData.hpp"
#include "VHostList.hpp"
#include "InputSocket.hpp"
#include "OutputSocket.hpp"
#include "InCGI.hpp"
#include "OutCGI.hpp"
#include "OutCGI.hpp"
#include "OutputSocket.hpp"
#include "VirtualHost.hpp"
#include <queue>

class Connection
{
	public:
		Connection(int fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server);
		~Connection();

		void			process();
		
		void			setIsChildren();
		bool			getIsChildren();

		InputSocket*	getInputSocket();
		OutputSocket*	getOutputSocket();

		InCGI*			getInCGI();
		void			setInCGI(InCGI*);

		OutCGI*			getOutCGI();
		void			setOutCGI(OutCGI*);

		int				getCgiPid();
		void			setCgiPid(int pid);

		void			add(ASocket*, int event);
		void			remove(ASocket*);
		void			removeFromInterestList(ASocket* abstractSocket);
		void			addToInterestList(ASocket* abstractSocket, int flags);

		uint8_t			*getPeerAddr();
		uint16_t		getPeerPort();
		uint16_t		getLocalPort();

		const VHostList& getHostList() const;

		time_t			getStartTime() const;

		const VirtualHost	*getVHost();
		void				setVHost();

	private:
		Connection();
		Connection(const Connection& other);
		const Connection& operator=(const Connection& other);

		// used to keep track of memory usage by each
		// connected client, in order to chose which connection 
		// to abort first;
		size_t				memoryUsage;

		// used to timeout request
		time_t				startTime;

		const VirtualHost	*vHost;

		uint8_t				peerAddr[4];
		uint16_t			peerPort;
		uint16_t			localPort;

		RequestMetaData		requestMetaData;
		int					state;

		InputSocket			inputSocket;
		OutputSocket		outputSocket;

		int					cgiPid;
		InCGI				*inCGI;
		OutCGI				*outCGI;

		Server&				server;
};

#endif
