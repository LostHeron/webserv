/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFactory.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:47:34 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 22:33:01 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__AFACTORY_HPP__
# define	__AFACTORY_HPP__

# include <stdint.h>
# include <iostream>

template	<class ABase>
class		AFactory: public ABase
{
	public:
		AFactory(void);
		AFactory(const AFactory<ABase> &cpy);
		virtual ~AFactory(void);

		ABase					*createElement(void) const;
	                        
	protected:              
		typedef ABase			*(*_constructor)(void);
		typedef uint8_t			(*_test)(void);

		virtual ABase			*_newElement(void) const = 0;
		virtual uint8_t			_determineElement(void) const = 0;
		void					_runTests(void);

		static _constructor		_constructorsArray[];
		static _test			_testsArray[];
		const unsigned int		_testsQty;
		uint16_t				_testsStatus;


};

# include "AFactory.tpp"

#endif
