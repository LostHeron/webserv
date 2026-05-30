/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutCGI.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:43:02 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 11:48:54 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTCGI_HPP
# define OUTCGI_HPP

#include "ASocket.hpp"
#include "Server.hpp"
#include <string>

class OutCGI: public ASocket
{
	public:
		OutCGI(int fd, Server& server);
		~OutCGI();

		void process();

	protected:

	private:
		OutCGI();
		OutCGI(const OutCGI& other);
		const OutCGI& operator=(const OutCGI& other);

		const std::string cgi_out_buffer;
};


OutCGI::OutCGI(int fd, Server& server):
	ASocket(server)
{
	this->fd = fd;
}

OutCGI::~OutCGI()
{
}

void OutCGI::process()
{
	if (cgi_out_buffer == "")
	{
		// here we should then read data from fd
		// else do nothing while buffer has not been clear 
		// from another process
	}
	return ;
}

#endif // !OUTCGI_HPP
