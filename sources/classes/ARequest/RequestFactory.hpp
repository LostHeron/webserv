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

# include "GETReq.hpp"
# include "POSTReq.hpp"
# include "DELETEReq.hpp"

class	RequestFactory: public AFactory<ARequest>
{
	public:
		RequestFactory(void);
		RequestFactory(const RequestFactory &cpy);
		~RequestFactory(void);

		RequestFactory	&operator=(const RequestFactory &assign);

		void						execute(void);


	private:
		const _constructor			*_getConstructors(void) const;
		uint8_t						_determineElement(void) const;

		template					<class Derived>
		static ARequest				*_newElement(const ARequest &tmp);

		uint8_t						_checkHeader(void) const;
		uint8_t						_checkBody(void) const;

		static const _constructor	_constructorsArray[];

};

# include "RequestFactory.tpp"

#endif
