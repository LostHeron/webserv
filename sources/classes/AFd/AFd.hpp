/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:21:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/10 17:32:22 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFD_HPP
# define AFD_HPP

#include "Server.hpp"

class AFd
{
	public:
		AFd(Server& other);
		virtual ~AFd();

		virtual void	process() = 0;

	protected:
		Server&	server;
		int		fd;

	private:
		AFd();
		AFd(const AFd& other);
		AFd& operator=(const AFd& other);
};

#endif
