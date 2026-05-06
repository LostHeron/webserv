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

#ifndef		__AMESSAGE_HPP__
# define	__AMESSAGE_HPP__

# include <iostream>

class	AMessage
{
	public:
		// AMessage(void);
		AMessage(const int &fd);
		AMessage(const AMessage &cpy);
		virtual ~AMessage(void);

		const int		&getFd(void) const;
		// void			setFd(const int &fd);

	protected:
		const int		_fd;
};

#endif
