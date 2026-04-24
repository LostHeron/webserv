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

class	GETReq: public ARequest // only GET ?
{
	public:
		GETReq(void);
		GETReq(const std::string &type, const std::string &header, const std::string &body);
		GETReq(const ARequest &cpy);
		GETReq(const GETReq &cpy);
		~GETReq(void);

		// GETReq		&operator=(const GETReq &assign);

		void		execute(void);
};

#endif
