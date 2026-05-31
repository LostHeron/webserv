/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToOutSocket.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:36:22 by jweber            #+#    #+#             */
/*   Updated: 2026/05/31 16:54:26 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOUTSOCKET_HPP
# define TOOUTSOCKET_HPP

#include "ASocket.hpp"
#include "Server.hpp"
#include <cstdio>
#include <string>
#include <unistd.h>

// this Structure is assumbed to be able to store all header in a
// single std::string or all model must be rethink;
class ToOutSocket: public ASocket
{
	public:
		ToOutSocket(int fd, bool& isLastBuffer, std::string& outputBuffer, Server &server);
		~ToOutSocket();

		void process();

	private:
		ToOutSocket();
		ToOutSocket(const ToOutSocket& other);
		const ToOutSocket& operator=(const ToOutSocket& other);

		int				state;

		bool&			isLastBuffer;
		std::string&	outputBuffer;
};

#endif // !TOOUTSOCKET_HPP
