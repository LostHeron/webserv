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
		virtual ABase			*CreateElement(void) const = 0;
	
	protected:
		template		<class CDerived>
		ABase			*newElement(void);

		virtual static ABase	*(*_constructors[])(void);
		virtual static bool		(_tests[])(void);


}

# include "AFactory.tpp"

#endif
