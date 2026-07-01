/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutgoingFd.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 18:04:18 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 18:49:09 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTGOINGFD_HPP
#define OUTGOINGFD_HPP

#include "ASocket.hpp"
#include "Connection.hpp"
#include <exception>
#include <queue>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

class OutgoingFd: public ASocket
{
	public:
		OutgoingFd(Connection* connection);
		~OutgoingFd();

		void	addBuffer(const std::string& newBuffer);
		void	process(void);

	private:
		OutgoingFd();
		OutgoingFd(const OutgoingFd& other);
		const OutgoingFd& operator=(const OutgoingFd& other);

		void	addToInterestList();
		void	removeFromInterestList();
		std::queue<std::string> buffers;
		std::string currentBuffer;
};

void	OutgoingFd::process(void)
{
	if (this->currentBuffer == "")
	{
		if (buffers.size() == 0)
			throw std::exception();
		this->currentBuffer = buffers.front();
		buffers.pop();
	}
	if (this->currentBuffer.size() > 0)
	{
		ssize_t nb_send = write(this->fd, this->currentBuffer.data(), this->currentBuffer.size());
		if (nb_send < 0)
			std::cerr << "An error occured while sending data to server\n";
		else
		{
			// used to transform this->buf = "salut toi le boss", 
			// whith nb_send = 2 to this->buf = "lut toi le boss"
			this->currentBuffer = std::string(this->currentBuffer, nb_send);
			if (this->currentBuffer == "" && this->buffers.size() == 0)
				this->removeFromInterestList();
		}
	}
}

void OutgoingFd::addBuffer(const std::string& newBuffer)
{
	if (this->buffers.size() == 0)
		this->addToInterestList();
	this->buffers.push(newBuffer);
}

void OutgoingFd::removeFromInterestList()
{
	this->getConnection()->removeFromInterestList(this);
}

void OutgoingFd::addToInterestList()
{
	this->getConnection()->addToInterestList(this, EPOLLOUT);
}

#endif
