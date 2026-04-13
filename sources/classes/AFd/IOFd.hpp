/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 13:46:19 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOFD_HPP
# define IOFD_HPP

#include "AFd.hpp"
#include "Server.hpp"

class IOFd: public AFd
{
	public:
		IOFd(int fd);
		~IOFd();

		void	process();
		bool	fail();
		
	protected:

	private:
		IOFd(const IOFd& other);
		IOFd& operator=(const IOFd& other);

		int	status;
};

#endif
