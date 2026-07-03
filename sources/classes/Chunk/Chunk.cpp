/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 12:45:51 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 13:55:27 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "status.hpp"

Chunk::Chunk():
	status(SUCCESS),
	state(0),
	lastChunk(false),
	finished(false),
	totalBlocksSize(0)
{
	process_functions[0] = &Chunk::process_size;
	process_functions[1] = &Chunk::process_CR;
	process_functions[2] = &Chunk::process_LF;
	process_functions[3] = &Chunk::process_data;
	process_functions[4] = &Chunk::process_CR;
	process_functions[5] = &Chunk::process_LF;
}

Chunk::~Chunk()
{
}

bool	Chunk::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}

bool		Chunk::isFinished()
{
	return (this->finished);
}

size_t		Chunk::getTotalSize()
{
	return (this->totalBlocksSize);
}

std::string	Chunk::getBuffer()
{
	std::string tmp = this->chunkBlocks.front();
	this->chunkBlocks.pop();
	return (tmp);
}

void	Chunk::process(std::string& buffer)
{
	while (buffer.size() != 0)
	{
		if (this->finished == true)
			return ;
		(this->*process_functions[this->state])(buffer);
		if (this->status == FAILURE)
			return ;
	}
}

void	Chunk::process_size(std::string& buffer)
{
	size_t	position = buffer.find("\r\n");
	if (position == std::string::npos)
	{
		this->sizeUnformatted.append(buffer);
	}
	else
	{
		this->sizeUnformatted.append(buffer, 0, position + 1);
		buffer = std::string(buffer, position);
		size_t	to_remove = this->sizeUnformatted.find_last_not_of("\r\n");
		this->sizeUnformatted = std::string(this->sizeUnformatted, 0, to_remove + 1);
	
		char *end;
		this->currentBlockSize = strtoll(this->sizeUnformatted.data(), &end, 16);
		if (end == this->sizeUnformatted.data() ||
			this->currentBlockSize == std::numeric_limits<size_t>::max() ||
			static_cast<size_t>(end - this->sizeUnformatted.data()) != this->sizeUnformatted.size()
			)
		{
			this->status = FAILURE;
			return ;
		}

		this->currentBlock.clear();
		this->currentBlock.reserve(this->currentBlockSize);
		this->state++;
	}
}

void Chunk::process_CR(std::string& buffer)
{
	if (buffer.size() > 0)
	{
		if (buffer.at(0) != '\r')
		{
			this->status = FAILURE;
			return ;
		}
		else
		{
			buffer = std::string (buffer, 1);
			this->state++;
		}
	}
}

void Chunk::process_LF(std::string& buffer)
{
	if (buffer.size() > 0)
	{
		if (buffer.at(0) != '\n')
		{
			this->status = FAILURE;
			return ;
		}
		else
		{
			buffer = std::string (buffer, 1);
			if (lastChunk == true)
				finished = true;
			this->state = (this->state + 1) % 6;
		}
	}
}

void	Chunk::process_data(std::string& buffer)
{
	if (this->currentBlockSize == 0)
	{
		this->lastChunk = true;
		this->state++;
		return ;
	}

	if (this->currentBlock.size() + buffer.size() > this->currentBlockSize)
	{
		size_t	to_append = this->currentBlockSize - this->currentBlock.size();
		this->currentBlock.append(buffer, 0, to_append);
		buffer = std::string(buffer, to_append);
		this->chunkBlocks.push(this->currentBlock);
		this->totalBlocksSize += this->currentBlockSize;
		//TODO if (this->totalBlockSize >= max_body_size)
		//	then we should send a 413
		this->currentBlock.clear();
		this->sizeUnformatted.clear();
		this->state++;
	}
	else
	{
		this->currentBlock.append(buffer);
		buffer.clear();
	}
}

std::ostream&	operator<<(std::ostream& os, Chunk& chunk)
{
	os << "chunck total block size = " << chunk.totalBlocksSize << "\n";
	return (os);
}
