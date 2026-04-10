/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFactory.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:29:02 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/10 17:49:56 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AFactory.hpp"

void	AFactory::_runTest(void)
{
	for (int i = 0; i < this->_testsQty; ++i)
	{
		this->_testsStatus |= (this->_testsArray[i] << i);
	}
}
