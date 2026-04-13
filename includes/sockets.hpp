/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 13:28:54 by jweber            #+#    #+#             */
/*   Updated: 2026/04/13 13:29:48 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

#include "Server.hpp"
#include <stdint.h>

void	CreateFd(int fd, Server& server);
void	CreateFd(uint16_t port, uint32_t addr, Server& server);

#endif
