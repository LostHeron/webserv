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
		AMessage(void);
		AMessage(const std::string &message, const std::string &target, const std::string &sender);
		AMessage(const AMessage &cpy);
		~AMessage(void);

		const std::string		&getMessage(void) const;
		const std::string		&getTarget(void) const;
		const std::string		&getSender(void) const;

		void					setMessage(const std::string &message);
		void					setTarget(const std::string &target);
		void					setSender(const std::string &sender);

	protected:
		std::string				_message;
		std::string				_target;
		std::string				_sender;
};

#endif
