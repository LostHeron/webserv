/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:51:42 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__REQUESTFACTORY_HPP__
# define	__REQUESTFACTORY_HPP__

# include "AFactory.hpp"
# include "ARequest.hpp"

# include "IOFd.hpp"

# include "GETReq.hpp"
# include "POSTReq.hpp"
# include "DELETEReq.hpp"
# include "UNKNOWNReq.hpp"

class	RequestFactory:
	public ARequest,
	public AFactory<ARequest>
{
	public:
		RequestFactory(const IOFd &IOMessage);
		RequestFactory(const RequestFactory &cpy);
		~RequestFactory(void);

		RequestFactory				&operator=(const RequestFactory &assign);

		Response					execute(void);
		ARequest					*createElement(void) const;


	private:
		const _constructor			*_getConstructors(void) const;
		int8_t						_determineElement(void) const;

		template					<class Derived>
		static ARequest				*_newElement(const ARequest &tmp);

		uint8_t						_checkHeader(void) const;
		uint8_t						_checkBody(void) const;

		static const _constructor	_constructorsArray[];

};

# include "RequestFactory.tpp"

#endif
