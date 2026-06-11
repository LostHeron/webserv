/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InCGI.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:20:07 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 11:31:17 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCGI_HPP
# define INCGI_HPP

#include "ASocket.hpp"
#include "Server.hpp"

class InCGI: public ASocket
{
	public:
		InCGI(int fd, size_t bodySize, std::string& newInputBuffer, Connection* connection);
		~InCGI();

		void	process();

	protected:

	private:
		InCGI();
		InCGI(const InCGI& other);
		const InCGI&	operator=(const InCGI&other);

		size_t			nbToSend;
		size_t			nbSent;
		std::string&	inputBuffer;
};

#endif 
