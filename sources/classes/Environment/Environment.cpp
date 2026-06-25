/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Environment.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 14:03:07 by jweber            #+#    #+#             */
/*   Updated: 2026/06/25 14:03:51 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Environment.hpp"
#include <string>
#include <cstring>

char **Environment::getEnvp(void)
{
	return (this->envp);
}

Environment::Environment(const std::string& script_name, InputSocket& inputSocket)
{
	inputSocket.updateCgiEnvp(vec_envp, script_name);

	formatted_envp.reserve(vec_envp.size() + 1);
	for (size_t i = 0; i < vec_envp.size(); i++)
	{
		char *tmp = new char[vec_envp.at(i).size() + 1];
		std::memcpy(tmp, vec_envp.at(i).data(), vec_envp.at(i).size());
		tmp[vec_envp.at(i).size()] = '\0';
		formatted_envp.push_back(tmp);
	}
	formatted_envp.push_back(NULL);
	envp = static_cast<char **>(formatted_envp.data());
	return ;
}

Environment::~Environment()
{
	for (size_t i = 0; i < formatted_envp.size(); i++)
	{
		delete [] formatted_envp.at(i);
	}
}

