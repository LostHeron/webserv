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
# include <dirent.h>
# include <errno.h>
# include <sys/types.h>

class	GETReq: public ARequest
{
	public:
		GETReq(const ARequest &src);
		GETReq(const GETReq &cpy);
		~GETReq(void);

	private:
		void		_execute(Response &resp, const VirtualHost::UriInfo &uriInfo);
};

#endif
