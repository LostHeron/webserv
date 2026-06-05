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
		Response(const int fd);
		Response(uint16_t errCode);
		Response(const Response &cpy);
		~Response(void);

		Response			&operator=(const Response &assign);

		const uint16_t		&getStatus(void) const;
		const int			&getResourceFd(void) const;
		const std::string	&getContent(void) const;

		void				setStatus(const uint16_t &status);
		void				setResourceFd(const int &resourceFd);
		void				setContent(const std::string &content);

	protected:
		uint16_t			_status;
		int					_resourceFd;
		std::string			_content;
};

#endif
