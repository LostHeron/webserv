/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETEReq.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:43:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__DELETEREQ_HPP__
# define	__DELETEREQ_HPP__

# include "ARequest.hpp"

class	DELETEReq: public ARequest // only DELETE ?
{
	public:
		DELETEReq(void);
		DELETEReq(const DELETEReq &cpy);
		~DELETEReq(void);

		// DELETEReq		&operator=(const DELETEReq &assign);

		void		execute(void);
};

#endif
