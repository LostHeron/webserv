/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFactory.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:54:35 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 18:09:37 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__AFACTORY_TPP__
# define	__AFACTORY_TPP__

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
	for (int i = 0; i < this->_testsQty; ++i)
	{
		this->_testsStatus |= (AFactory<ABase>::_testsArray[i] << i);
	}
}
template	<class ABase>
ABase		*AFactory<ABase>::createElement(void) const
{
	int	i = this->_determineElement();

	if (i == 4)
		return (NULL);
	return (AFactory<ABase>::_constructorsArray[i]);
}

#endif
