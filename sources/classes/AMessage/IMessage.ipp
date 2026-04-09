/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.ipp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:18:28 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:29:32 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__IMESSAGE_IPP__
# define	__IMESSAGE_IPP__

# include <string>

class	IMessage
{
	public:
		virtual ~IMessage(void) {};

		virtual const std::string		&getMessage(void) const = 0;
		virtual const std::string		&getSender(void) const = 0;
		virtual const std::string		&getTarget(void) const = 0;
};

#endif
