/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutCGI.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 11:43:02 by jweber            #+#    #+#             */
/*   Updated: 2026/06/01 15:37:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTCGI_HPP
# define OUTCGI_HPP

#include "ASocket.hpp"
#include "Server.hpp"
#include "typedef.hpp"
#include <string>

class InputSocket;
class OutputSocket;

class OutCGI: public ASocket
{
	public:
		OutCGI(int fd, InputSocket& is, OutputSocket& os, Server& server);
		~OutCGI();

		void process();
		void process_headers(size_t &start);
		void process_body(size_t &start);
		void update_buffer();

	protected:

	private:
		OutCGI();
		OutCGI(const OutCGI& other);
		const OutCGI& operator=(const OutCGI& other);

		InputSocket		&is;
		OutputSocket	&os;	
		int				state;
		string_map		headers;
		std::string 	cgi_out_buffer;
		std::string 	last_line;
};


#endif // !OUTCGI_HPP
