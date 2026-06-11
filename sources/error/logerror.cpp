/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logerror.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 18:46:33 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 17:35:04 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cerrno>
#include <cstring>

void	logerror(const char *str, int errno_value)
{
	std::string error_msg(strerror(errno_value));
	std::cerr << str << ": " << error_msg << "\n";
}
