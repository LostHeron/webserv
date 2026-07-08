/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 10:39:28 by jweber            #+#    #+#             */
/*   Updated: 2026/07/07 14:46:30 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

#include <csignal>
#include <cstdlib>
#include <ostream>
#include <queue>
#include <string>

class Chunk
{
	public:
		Chunk();
		~Chunk();

		void		process(std::string& buffer);
		std::string	getBuffer();
		bool		fail();
		bool		isFinished();
		size_t		getTotalSize();
		int			getStatus();
		void		setMaxBodySize(size_t newMaxBodySize);
		friend std::ostream&	operator<<(std::ostream& os, Chunk& chunk);

	private:
		int						status;
		int						state;
		bool					lastChunk;
		bool					finished;
		size_t					totalBlocksSize;
		size_t					maxBodySize;
		std::queue<std::string>	chunkBlocks;
		void (Chunk::*process_functions[6])(std::string& buffer);

		std::string		sizeUnformatted;
		size_t			currentBlockSize;
		std::string		currentBlock;

		void	process_size(std::string& buffer);
		void	process_data(std::string& buffer);
		void	process_CR(std::string& buffer);
		void	process_LF(std::string& buffer);
};


#endif
