/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:00 by jweber            #+#    #+#             */
/*   Updated: 2026/04/09 11:48:22 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
# define LISTEN_HPP

#include <netinet/in.h>
#include <stdint.h>
#include <vector>

class Listen
{
	public:
		Listen(uint32_t	addres, uint16_t port);
		Listen(const Listen& other);
		~Listen();
	
		void	activate();
		bool	fail();

	protected:

	private:
		Listen();
		Listen&	operator=(const Listen& other);
		int status;
		struct sockaddr_in addr_data;
		int	fd;
};

#endif
