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
# include "IOFd.hpp"

class	ARequest: public AMessage
{
	public:
		// ARequest(void);
		ARequest(const IOFd &IOMessage);
		// ARequest(const AMessage &message, const std::string &type, const std::string &header, const std::string &body);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest			&operator=(const ARequest &assign);

		virtual void		execute(void) = 0;

		// const std::string	&getType(void) const;
		// const std::string	&getHeader(void) const;
		// const std::string	&getBody(void) const;
		//
		// void	setType(const std::string &type);
		// void	setHeader(const std::string &header);
		// void	setBody(const std::string &body);

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
