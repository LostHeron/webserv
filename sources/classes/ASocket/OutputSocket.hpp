/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 10:36:09 by jweber            #+#    #+#             */
/*   Updated: 2026/06/05 14:51:10 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTSOCKET_HPP
# define OUTPUTSOCKET_HPP

#include "ASocket.hpp"

class OutputSocket: public ASocket
{
	public:
		OutputSocket(int socket_fd, Connection* connection);
		~OutputSocket();

		void	process();

		std::string&	getOutputBuffer();
		bool&			getIsLastBuffer();
		void			updateOutputBuffer();
		void			setup(int newRessourceFd, const std::string& firstBuffer);
		void			end();

	protected:

	private:
		OutputSocket();
		OutputSocket(const OutputSocket& other);
		const OutputSocket&	operator=(const OutputSocket& other);

		int			ressourceFd;
		bool		ready;
		bool		isLastBuffer;
		std::string	outputBuffer;
};

#endif // !OUTPUTSOCKET_HPP
