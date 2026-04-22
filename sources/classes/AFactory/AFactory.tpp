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

// template	<class ABase>
// typename AFactory<ABase>::_constructor	AFactory<ABase>::_constructorsArray[] = {0};
//
// template	<class ABase>
// typename AFactory<ABase>::_test			AFactory<ABase>::_testsArray[] = {0};

// Constructors/Destructors ====================================================
template	<class ABase>
AFactory<ABase>::AFactory(void):
	ABase(),
	_testsQty(0),
	_testsStatus(0) {}

template	<class ABase>
AFactory<ABase>::AFactory(const AFactory<ABase> &cpy):
	ABase(cpy),
	_testsQty(cpy._testsQty),
	_testsStatus(cpy._testsStatus) {}

// Member functions ============================================================
template	<class ABase>
void		AFactory<ABase>::_runTests(void)
{
	const _test	*testsArray = this->_getTests();
	for (int i = 0; i < this->_testsQty; ++i)
	{
		this->_testsStatus |= (testsArray[i]() << i);
	}
}
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
