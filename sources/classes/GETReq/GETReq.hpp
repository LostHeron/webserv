/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:49:05 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__GETREQ_HPP__
# define	__GETREQ_HPP__

# include "ARequest.hpp"

class	GETReq: public ARequest
{
	public:
		GETReq(void);
		GETReq(const GETReq &cpy);
		~GETReq(void);

		GETReq					&operator=(const GETReq &assign);

		const uint8_t			&getType(void) const;
		const std::string		&getHeader(void) const;
		const std::string		&getBody(void) const;
}

#endif
