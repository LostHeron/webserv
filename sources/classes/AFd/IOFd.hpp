/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/10 16:53:32 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOFD_HPP
# define IOFD_HPP

#include "AFd.hpp"
#include "Server.hpp"

class IOFd: public AFd
{
	public:
		IOFd(int fd, Server& server);
		~IOFd();

		void process();
		
	protected:

	private:
		IOFd(const IOFd& other);
		IOFd& operator=(const IOFd& other);
};

#endif
