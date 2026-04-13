/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollStruct.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:30:39 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 15:23:14 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLSTRUCT_HPP
# define EPOLLSTRUCT_HPP

class AFd;

class EpollStruct
{
	public:
		EpollStruct();
		~EpollStruct();

		bool	fail();
		void	add(AFd *fd);
		int		getFd();

	protected:

	private:
		int	fd;
		int	status;
		EpollStruct(const EpollStruct& other);
		EpollStruct& operator=(const EpollStruct& other);
};

#endif // !EPOLLSTRUCT_HPP
