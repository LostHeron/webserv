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

template	<class ABase>
class		AFactory
{
	public:
		virtual ~AFactory(void);
		virtual ABase						*createElement(void) const = 0;
		virtual uint8_t						determineElement(void) const = 0;
	                            		
	protected:                  		
		typedef ABase						*(*_constructor)(void);
		typedef uint8_t						(*_test)(void);
                                		
		template							<class CDerived>
		virtual ABase						*_newElement(void) = 0;

		virtual static const _constructor	_constructorsArray[];
		virtual static const _test			_testsArray[];


}

# include "AFactory.tpp"

#endif
