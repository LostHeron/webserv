/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:01:33 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 13:49:27 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AFd.hpp"
#include "Server.hpp"

AFd::AFd(Server& server):
	server(server)
{
}

AFd::~AFd()
{
}

int	AFd::getFd()
{
	return (this->fd);
}
