/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:01:33 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 18:35:52 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AFd.hpp"
#include "Server.hpp"
#include "status.hpp"
#include <unistd.h>

AFd::AFd(Server& server):
	fd(-1),
	memoryUsage(0),
	status(SUCCESS),
	server(server)
{
}

AFd::~AFd()
{
	close(this->fd);
}

int	AFd::getFd() const
{
	return (this->fd);
}

bool	AFd::fail()
{
	if (this->status != SUCCESS)
		return (true);
	else
		return (false);
}
