/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:39:01 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 12:45:51 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pipe.hpp"

Pipe::Pipe()
{
	this->fd[0] = -1;
	this->fd[1] = -1;
	if (pipe(this->fd) < 0)
	{
		throw Pipe::PipeFailure();
	}
}

Pipe::~Pipe()
{
	if (this->fd[0] >= 0)
		close(fd[0]);

	if (this->fd[1] >= 0)
		close(fd[1]);
}

int	Pipe::getWriteEnd() const
{
	return  this->fd[1];
}

int Pipe::getReadEnd() const
{
	return  this->fd[0];
}

void Pipe::closeWriteEnd()
{
	if (this->fd[1] >= 0)
	{
		close(this->fd[1]);
		this->fd[1] = -1;
	}
}

void Pipe::closeReadEnd()
{
	if (this->fd[0] >= 0)
	{
		close(this->fd[0]);
		this->fd[0] = -1;
	}
}

const char *Pipe::PipeFailure::what() const throw() 
{
	return ("pipe syscall failed");
}
