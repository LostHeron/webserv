/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFactory.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:54:35 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 18:26:32 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__AFACTORY_TPP__
# define	__AFACTORY_TPP__

// Constructors/Destructors ====================================================
template	<class ABase>
AFactory<ABase>::AFactory(void) {}

template	<class ABase>
AFactory<ABase>::~AFactory(void) {}

// template	<class ABase>
// AFactory<ABase>::AFactory(const AFactory<ABase> &cpy):
// 	ABase(cpy) {}
//
// Member functions ============================================================
template	<class ABase>
ABase		*AFactory<ABase>::createElement(void) const
{
	const _constructor	*constructorsArray = this->_getConstructors();
	const int			i = this->_determineElement();

	if (i == 4)
		return (NULL);
	return (constructorsArray[i](*this));
}

#endif
