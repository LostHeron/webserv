/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaRequest.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 21:40:27 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__REQUESTFACTORY_HPP__
# define	__REQUESTFACTORY_HPP__

# include "GETReq.hpp"
# include "POSTReq.hpp"
# include "DELETEReq.hpp"

class	RequestFactory
{
	public:
		static ARequest	*buildRequest(void) const;

	private:
		uint8_t			_type;
		std::string		_header;
		std::string		_body;

		bool			_checkHeader(const std::string &header) const;
		bool			_checkBody(const std::string &body) const;
	
}

# include "RequestFactory.tpp"

#endif
