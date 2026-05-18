/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResponse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:37:23 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__RESPONSE_HPP__
# define	__RESPONSE_HPP__

# include "AMessage.hpp"
# include <stdint.h>

class	Response: public AMessage
{
	public:
		Response(const int &fd);
		Response(const Response &cpy);
		~Response(void);

		Response		&operator=(const Response &assign);

		// void	send(void); // remove, metadata to store and then to be processed (queue) by epoll
		//

		const uint16_t	&getStatus(void) const;
		void			setResourceFd(const int &resourceFd);

	protected:
		// metadata
		uint16_t		_status;
		int				_resourceFd;
		// if no resources to read during response, build pipe, resource fd becomes read side, response write onto write side the error (bad request/invalide resouce...)
};

#endif
