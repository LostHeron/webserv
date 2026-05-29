/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:29 by jweber            #+#    #+#             */
/*   Updated: 2026/05/28 17:47:18 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTSOCKET_HPP
# define INPUTSOCKET_HPP

# include "ASocket.hpp"
# include "Server.hpp"
# include <map>
# include <ostream>
# include <vector>
# include <string>
# include <stdint.h>

# define IOFD_MAX_SIZE 5000

typedef std::map<std::string, std::vector<std::string> > string_map;

class InputSocket: public ASocket
{
	public:
		friend std::ostream& operator<<(std::ostream& os, const InputSocket& iofd);
		InputSocket(int fd, uint16_t local_port, const struct sockaddr_in& addr, Server& server);
		
		~InputSocket();

		const std::string					&getMethod(void) const;
		const std::string					&getUri(void) const;
		const std::string					&getVersion(void) const;
		const string_map					&getHeaders(void) const;

		void	process();
		
	protected:

	private:
		InputSocket(const InputSocket& other);
		InputSocket& operator=(const InputSocket& other);

		void (InputSocket::*process_functions[10])(std::string& buf, size_t& pos);

		// used to know which state the program is in
		int					state;	

		uint16_t			local_port;
		uint16_t			peer_port;
		uint8_t				addr[4];

		// identify which method the client tries to reach
		std::string					method;
		void						process_method(std::string&, size_t& pos);

		// identify which uri the client tries to reach
		std::string					uri;
		void						process_uri(std::string&, size_t& pos);

		// identify which version of HTTP the client tries to reach
		std::string					version;
		void						process_version(std::string&, size_t& pos);

		// vector of vector of char, where vector of char
		// represent each line, and the vector of vector of lines
		// represent all the lines in the header
		//std::vector< std::string >					header;
		string_map					headers;
		void						process_headers(std::string&, size_t& pos);

		void						process_body(std::string&, size_t& pos);

		void						process_skip_sp(std::string&, size_t& pos);
		void						process_request(std::string&, size_t& pos);
};

void		send_bad_request(int fd, int& status);
size_t		getDelimPosition(const std::string& str, size_t start, const std::vector<std::string>& delims);

#endif
