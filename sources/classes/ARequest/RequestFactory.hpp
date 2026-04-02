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

	private:
		uint8_t			_type;
		std::string		_header;
		std::string		_body;
		
		uint8_t			_checkType(void) const;
		uint8_t			_checkHeader(void) const;
		uint8_t			_checkBody(void) const;
	
};

#endif
