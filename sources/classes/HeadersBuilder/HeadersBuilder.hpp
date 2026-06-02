/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 11:16:27 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 17:00:33 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERSBUILDER_HPP
# define HEADERSBUILDER_HPP

#include <cstring>
#include <ctime>
#include <string>

class HeadersBuilder
{
	public:
		HeadersBuilder();
		~HeadersBuilder();

		HeadersBuilder&	initialize();
		HeadersBuilder&	buildStatusLine(const std::string& version, int errorCode);
		HeadersBuilder&	buildDate();
		HeadersBuilder& buildHeaderKeyValue(const std::string& key, const std::string& value);
		HeadersBuilder&	buildCRLF();
		HeadersBuilder&	buildBody(int errorCode);
		std::string			build();
	private:
		HeadersBuilder(const HeadersBuilder& other);
		const HeadersBuilder& operator=(const HeadersBuilder& other);
		std::string response;
};

#endif
