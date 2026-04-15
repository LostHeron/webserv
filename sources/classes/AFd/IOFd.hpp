/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOFd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 16:06:29 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:31:25 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOFD_HPP
# define IOFD_HPP

# include "AFd.hpp"
# include "Server.hpp"
#include <ostream>
# include <vector>
# include <string>

# define IOFD_MAX_SIZE 2049

class IOFd: public AFd
{
	public:
		friend std::ostream& operator<<(std::ostream& os, const IOFd& iofd);
		IOFd(int fd, Server& server);
		~IOFd();

		void	process();
		
	protected:

	private:
		IOFd(const IOFd& other);
		IOFd& operator=(const IOFd& other);

		// enum used to know in which state the fd is in
		// at start it is in 'METHOD' state, whiche means
		// incoming information will be used to fill in 
		// method until white space are encoutered
		enum States {
			METHOD,
			URI,
			VERSION,
			HEADER,
			BODY,
			DISCARD,
		};

		void (IOFd::*process_functions[6])(std::string& buf, size_t pos);

		// used to know which state the program is in
		int state;	

		// identify which method the client tries to reach
		std::string									method;
		void										process_method(std::string&, size_t pos);

		// identify which uri the client tries to reach
		std::string									uri;
		void										process_uri(std::string&, size_t pos);

		// identify which version of HTTP the client tries to reach
		std::string									version;
		void										process_version(std::string&, size_t pos);

		// vector of vector of char, where vector of char
		// represent each line, and the vector of vector of lines
		// represent all the lines in the header
		std::vector< std::vector<unsigned char> >	header;
		void										process_header(std::string&, size_t pos);

		// body of the request, must be sur a 'content length' is present
		// in the request to know how much data to store in the body !
		// also in here do we store the data send ? or do we wait until
		// we know what to do with the request ...
		std::vector<unsigned char>					body;
		void										process_body(std::string&, size_t pos);

		// if there's still data to process after having retrieved the entire
		// body, server should close the connection with a bad request response
		void										process_abort(std::string&, size_t pos);
};


#endif
