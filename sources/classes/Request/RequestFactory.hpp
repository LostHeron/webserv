/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/28 16:26:42 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__REQUESTFACTORY_HPP__
# define	__REQUESTFACTORY_HPP__

# include "AFactory.hpp"
# include "ARequest.hpp"

# include "InputSocket.hpp"

# include "GETReq.hpp"
# include "POSTReq.hpp"
# include "DELETEReq.hpp"
# include "UNKNOWNReq.hpp"
# include "PUTReq.hpp"
# include "VirtualHost.hpp"

class	RequestFactory:
	public ARequest,
	public AFactory<ARequest>
{
	public:
		RequestFactory(const InputSocket &IOMessage, const VirtualHost& vhost);
		RequestFactory(const RequestFactory &cpy);
		~RequestFactory(void);

		RequestFactory				&operator=(const RequestFactory &assign);

		ARequest					*createElement(void) const;


	private:
		void						_execute(Response &resp, const VirtualHost::UriInfo &uriInfo);
		const _constructor			*_getConstructors(void) const;
		int8_t						_determineElement(void) const;

		template					<class Derived>
		static ARequest				*_newElement(const ARequest &tmp);

		static const _constructor	_constructorsArray[];

};

# include "RequestFactory.tpp"

#endif
