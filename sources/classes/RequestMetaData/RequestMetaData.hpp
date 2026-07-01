/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestMetaData.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:41:53 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 17:46:23 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTMETADATA_HPP
# define REQUESTMETADATA_HPP

# include <string>

class RequestMetaData
{
	public:

	private:
		std::string		method;
		std::string		uri;
		std::string		queryString;
		std::string		version;
		string_map		headers;
};

#endif 
