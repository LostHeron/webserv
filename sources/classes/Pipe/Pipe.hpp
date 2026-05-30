/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:31:57 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 13:02:33 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_HPP
# define PIPE_HPP

#include <exception>

class Pipe
{
	public:
		Pipe();
		~Pipe();

		int	getWriteEnd() const;
		int getReadEnd() const;

		void closeWriteEnd();
		void closeReadEnd();

		class PipeFailure: public std::exception
		{
			const char * what() const throw();
		};
		
	private:
		Pipe(const Pipe& other);
		const Pipe& operator=(const Pipe& other);
		int fd[2];
};

#endif // !PIPE_HPP
