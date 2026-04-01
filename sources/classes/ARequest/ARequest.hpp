/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:58:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__AREQUEST_HPP__
# define	__AREQUEST_HPP__

# include "AMessage.hpp"

class	ARequest: public AMessage
{
	public:
		ARequest(void);
		ARequest(const ARequest &cpy);
		~ARequest(void);

		ARequest					&operator=(const ARequest &assign);

		virtual const uint8_t		&getType(void) const = 0;
		virtual const std::string	&getHeader(void) const = 0;
		virtual const std::string	&getBody(void) const = 0;

		enum						e_reqType
		{
			GET,
			POST,
			DELETE,
			UNKNOWN,
			TYPE_QTY
		};

	protected:
		static const uint8_t	_type;
		const std::string		_header;
		const std::string		_body;
};

#endif
