/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 21:26:49 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 18:08:07 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__REQUESTFACTORY_TPP__
# define	__REQUESTFACTORY_TPP__

template		<class Derived>
ARequest		*RequestFactory::_newElement(void) const
{
	return (new Derived(this->_type, this->_header, this->_body));
}


#endif
