/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InputSocket.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:29 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 13:59:08 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTSOCKET_HPP
# define INPUTSOCKET_HPP

# include "ASocket.hpp"
# include "Response.hpp"
# include "Server.hpp"
# include "typedef.hpp"
# include <ostream>
# include <vector>
# include <string>
# include <stdint.h>

# define INPUTSOCKET_MAX_SIZE 5000

class ARequest;

class InputSocket: public ASocket
{
	public:
		InputSocket(int fd, Connection* connection);
		
		~InputSocket();

		const std::string	&getMethod(void) const;
		const std::string	&getUri(void) const;
		const std::string	&getVersion(void) const;
		const string_map	&getHeaders(void) const;
		string_map			&getHeadersNoConst(void);

		void	process();

		void	updateCgiEnvp(std::vector<std::string>&, const std::string& script_name, const std::string& pathInfo);

		friend std::ostream& operator<<(std::ostream& os, const InputSocket& inputSocket);

		void						launch_cgi(size_t nbToSend);
		
	protected:

	private:
		InputSocket(const InputSocket& other);
		InputSocket& operator=(const InputSocket& other);

		void (InputSocket::*process_functions[10])(size_t& pos);

		// used to know which state the program is in
		std::string			inputBuffer;
		int					state;	
		bool				endByBackslashR;

		// identify which method the client tries to reach
		std::string					method;
		void						process_method(size_t& pos);

		// identify which uri the client tries to reach
		std::string					uri;
		std::string					queryString;
		void						process_uri(size_t& pos);

		// identify which version of HTTP the client tries to reach
		std::string					version;
		void						process_version(size_t& pos);

		// vector of vector of char, where vector of char
		// represent each line, and the vector of vector of lines
		// represent all the lines in the header
		//std::vector< std::string >					header;
		std::string					lastLine;
		string_map					headers;
		void						process_headers(size_t& pos);

		size_t						bodySize;
		void						process_body(size_t& pos);
		size_t						nbSent;

		void						process_skip_sp(size_t& pos);
		void						process_request(size_t& pos);

		Response					*resp;
		ARequest					*req;
};

void	setup_response(int& status, int errorCode, Connection *connection);

#endif
