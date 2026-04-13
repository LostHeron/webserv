/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:21:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 13:49:03 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFD_HPP
# define AFD_HPP

#include "Server.hpp"

class AFd
{
	public:
		AFd();
		virtual ~AFd();

		int	getFd();

		virtual void	process() = 0;
		virtual	bool	fail() = 0;

	protected:
		int		fd;

	private:
		AFd(const AFd& other);
		AFd& operator=(const AFd& other);
};

#endif
