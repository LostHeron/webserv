/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:18:28 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:41:26 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__AMESSAGE_IPP__
# define	__AMESSAGE_IPP__

# include <IMessage.ipp>

class	AMessage: public IMessage
{
	protected:
		const std::string		_message;
		const std::string		_sender;
		const std::string		_target;
};

#endif
