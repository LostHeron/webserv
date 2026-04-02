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

class	ARequest: public AMessage
{
	public:
		ARequest(const uint8_t &type, const std::string &header, const std::string &body);
		ARequest(const ARequest &cpy);
		virtual ~ARequest(void);

		ARequest			&operator=(const ARequest &assign);

		virtual const void	execute(void) = 0;

		const uint8_t		&getType(void) const;
		const std::string	&getHeader(void) const;
		const std::string	&getBody(void) const;

		enum				e_reqType
		{
			GET,
			POST,
			DELETE,
			TYPE_QTY,
			UNKNOWN,
		};

	protected:
		const uint8_t		_type;
		const std::string	_header;
		const std::string	_body;
};

#endif
