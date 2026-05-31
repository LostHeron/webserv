/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 10:36:09 by jweber            #+#    #+#             */
/*   Updated: 2026/05/31 17:19:52 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTSOCKET_HPP
# define OUTPUTSOCKET_HPP

#include "ASocket.hpp"

class OutputSocket: public ASocket
{
	public:
		OutputSocket(int socket_fd, Server& server);
		~OutputSocket();

		void	process();

		std::string&	getOutputBuffer();
		bool&			getIsLastBuffer();
		void			end();

protected:

	private:
		OutputSocket();
		OutputSocket(const OutputSocket& other);

		bool		isLastBuffer;
		std::string	outputBuffer;
};

#endif // !OUTPUTSOCKET_HPP
