/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/05 14:39:51 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__RESPONSE_HPP__
# define	__RESPONSE_HPP__

# include "AMessage.hpp"
# include "VirtualHost.hpp"
# include <stdint.h>

class	Response: public AMessage
{
	public:
		Response(const int fd, bool isCGI);
		Response(uint16_t errCode, const VirtualHost &vHost);
		Response(const Response &cpy);
		~Response(void);

		Response							&operator=(const Response &assign);

		const uint16_t						&getStatus(void) const;
		std::pair<int, std::string>			&getResource(void);
		std::string							&getContent(void);
		bool								isCGI(void) const;

		void								setStatus(const uint16_t status);
		void								setResource(std::pair<int, std::string> &resource);
		void								setResourceFd(int fd);
		void								setResourcePath(std::string path);
		void								setContent(const std::string &content);
		void								setCGI(const bool isCGI);

		void								error(const VirtualHost &vHost);

	protected:
		uint16_t							_status;
		std::pair<int, std::string>			_resource;
		std::string							_content;
		bool								_cgi;
};

#endif
