/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:19:10 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef		__AREQUEST_HPP__
# define	__AREQUEST_HPP__

# include "AMessage.hpp"
# include "Response.hpp"
# include "IOFd.hpp"
# include <unistd.h>
# include <fcntl.h>


# define TEMP_ROOT "/home/abetemps/goinfre/webserv_root"

class	ARequest: public AMessage
{
	public:
		ARequest(const IOFd &IOMessage);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest							&operator=(const ARequest &assign);
                                			
		virtual Response					execute(void) = 0; // instant execute immediate actions, then build Response according to execution metadata, status...

		const std::string					&getMethod(void) const;
		const std::string					&getUri(void) const;
		const std::string					&getVersion(void) const;
		const std::vector< std::string >	&getHeader(void) const;
		const std::vector<unsigned char>	&getBody(void) const;

		enum				e_reqType
		{
			GET,
			POST,
			DELETE,
			TYPE_QTY,
			UNKNOWN,
		};

	protected:
		const std::string				  	_method;
		const std::string				  	_uri;
		const std::string				  	_version;
		const std::vector< std::string >	_header;
		const std::vector<unsigned char>	_body;
};

#endif
