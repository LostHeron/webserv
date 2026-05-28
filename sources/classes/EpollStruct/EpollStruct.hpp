/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollStruct.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:30:39 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 15:12:56 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLSTRUCT_HPP
# define EPOLLSTRUCT_HPP

class ASocket;

class EpollStruct
{
	public:
		EpollStruct();
		~EpollStruct();

		bool	fail();
		void	add(ASocket *fd, int event);
		void	remove(ASocket *fd);
		int		getFd();

	protected:

	private:
		int	fd;
		int	status;
		EpollStruct(const EpollStruct& other);
		EpollStruct& operator=(const EpollStruct& other);
};

#endif // !EPOLLSTRUCT_HPP
