/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 10:36:09 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:10:44 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUTSOCKET_HPP
# define OUTPUTSOCKET_HPP

#include "ASocket.hpp"
#include "Response.hpp"

// two possibility: 
//  1) This is not a CGI :
//		- generate headers and write them to the client
//		- read from the ressource fd and write
class OutputSocket: public ASocket
{
	public:
		OutputSocket(int socket_fd, Server& server);
		~OutputSocket();

		void	process();

		void	setup(const Response& res);

	protected:

	private:
		OutputSocket();
		OutputSocket(const OutputSocket& other);

		bool		ready;
		bool		send_headers;
		std::string	buf;
		int			ressourceFd;
		//int		socketFd;
};



#endif // !OUTPUTSOCKET_HPP
