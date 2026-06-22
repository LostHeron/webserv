/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTMLPageBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 19:25:11 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/01 19:29:45 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__HTMLPAGEBUILDER_HPP__
# define	__HTMLPAGEBUILDER_HPP__

# include <dirent.h>
# include <vector>
# include "HTTPStatus.cpp"

class	HTMLPageBuilder
{
	public:
		static const std::string	dirListingPage(DIR *directory, const std::string &uri);
		static const std::string	errorPage(const uint16_t errCode);


	private:
		static inline const std::string	_headerTitled(const std::string title);
		static inline const std::string	_unorderedList(std::vector<std::string> &v_content);
		static inline const std::string	_href(const std::string target, const std::string uri);
		static inline const std::string _tag(const std::string name, const std::string content, const bool newline);
		static inline void				_addTag(const std::string tag, std::vector<std::string> &content);

};

#endif
