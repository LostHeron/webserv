/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETEReq.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:43:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__DELETEREQ_HPP__
# define	__DELETEREQ_HPP__

# include "ARequest.hpp"

class	DELETEReq: public ARequest
{
	public:
		DELETEReq(const ARequest &cpy);
		DELETEReq(const DELETEReq &cpy);
		~DELETEReq(void);

		Response		execute(void);

	private:
		void		_execute(Response &resp, const VirtualHost::UriInfo &uriInfo);
};

#endif
