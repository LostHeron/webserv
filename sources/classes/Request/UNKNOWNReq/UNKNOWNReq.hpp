/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UNKNOWNReq.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:43:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__UNKNOWNREQ_HPP__
# define	__UNKNOWNREQ_HPP__

# include "ARequest.hpp"

class	UNKNOWNReq: public ARequest // only UNKNOWN ?
{
	public:
		// UNKNOWNReq(void);
		// UNKNOWNReq(const std::string &type, const std::string &header, const std::string &body);
		UNKNOWNReq(const ARequest &cpy);
		UNKNOWNReq(const UNKNOWNReq &cpy);
		~UNKNOWNReq(void);

		// UNKNOWNReq		&operator=(const UNKNOWNReq &assign);

		Response		execute(void);
};

#endif
