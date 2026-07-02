/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PUTReq.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:43:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__PUTREQ_HPP__
# define	__PUTREQ_HPP__

# include "ARequest.hpp"

class	PUTReq: public ARequest // only PUT ?
{
	public:
		// PUTReq(void);
		// PUTReq(const std::string &type, const std::string &header, const std::string &body);
		PUTReq(const ARequest &cpy);
		PUTReq(const PUTReq &cpy);
		~PUTReq(void);

		// PUTReq		&operator=(const PUTReq &assign);

	private:
		void		_execute(Response &resp, const VirtualHost::UriInfo &uriInfo);
		void		_uploadFile(Response &resp, const std::string &upPath);
};

#endif
