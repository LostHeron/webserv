/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/04/01 19:02:27 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <vector>
#include <string>

class VirtualHost
{
	public:

	private:
		std::vector<uint16_t>				ListeningPort;
		std::map<std::string, Location>		Locations;
		std::vector<std::string>			Hosts;
		std::map<int, std::string>			ErrorPage;
};
