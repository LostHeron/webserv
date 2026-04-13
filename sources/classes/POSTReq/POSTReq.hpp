/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POSTReq.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:42:41 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__POSTREQ_HPP__
# define	__POSTREQ_HPP__

# include "ARequest.hpp"

class	POSTReq: public ARequest // only POST ?
{
	public:
		POSTReq(void);
		POSTReq(const POSTReq &cpy);
		~POSTReq(void);

		// POSTReq		&operator=(const POSTReq &assign);

		void		execute(void);
};

#endif
