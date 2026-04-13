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

		void		execute(void);

	private:

		ARequest	*_newElement(void) const;
		uint8_t		_determineElement(void) const;

		enum			e_Tests
		{
			ISGET,
			ISPOST,
			ISDELETE,
			CHECKHEADER,
			CHECKBODY,
		};

		uint8_t			_isGet(void) const;
		uint8_t			_isPost(void) const;
		uint8_t			_isDelete(void) const;
		uint8_t			_checkHeader(void) const;
		uint8_t			_checkBody(void) const;
	
};

# include "RequestFactory.tpp"

#endif
