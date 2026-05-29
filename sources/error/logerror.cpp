/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:46:33 by jweber            #+#    #+#             */
/*   Updated: 2026/05/29 18:47:08 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cerrno>
#include <cstring>

void	logerror()
{
	std::string error_msg(strerror(errno));
	std::cerr << "read: " << error_msg << "\n";
}
