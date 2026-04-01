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

#ifndef		__ARESPONSE_HPP__
# define	__ARESPONSE_HPP__

class	AResponse: public AMessage
{
	public:
		AResponse(void);
		AResponse(const AResponse &cpy);
		~AResponse(void);

		AResponse		&operator=(const AResponse &assign);

		const uint16_t	&getStatus(void) const = 0;
		void			send(void) const = 0;

	protected:
		uint16_t		_status;

};

#endif
