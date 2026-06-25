/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Environment.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 14:02:08 by jweber            #+#    #+#             */
/*   Updated: 2026/06/25 14:03:00 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_HPP
# define ENVIRONMENT_HPP

#include <string>
#include "InputSocket.hpp"

class Environment
{
	public:
		Environment(const std::string& script_name, InputSocket& inputSocket);
		~Environment();

		char **getEnvp(void);
	
	private:
		Environment();
		Environment(const Environment& other);
		const Environment& operator=(const Environment& other);
	
		std::vector<std::string>	vec_envp;
		std::vector< char * >		formatted_envp;
		char						**envp;
};

#endif
