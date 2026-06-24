/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTMLPageBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 19:27:36 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/01 19:27:43 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTMLPageBuilder.hpp"

#include <algorithm>
#include <sstream>

// UTIL
inline std::string		concatVec(const std::vector<std::string> &vec)	
{
	std::vector<std::string>::const_iterator	it;
	std::string							result;

	for (it = vec.begin(); it < vec.end(); ++it)
		result += *it;

	return (result);
}

// PUBLIC
const std::string	HTMLPageBuilder::dirListingPage(DIR *directory, const std::string &uri)
{
	std::string		content("<!DOCTYPE html>");
	struct dirent	*entry = readdir(directory);
	std::vector<std::string>	entries;

	content += HTMLPageBuilder::_headerTitled("Index of " + uri);
	content += HTMLPageBuilder::_tag("h1", "Index of " + uri, false);
	while (entry)
	{
		std::string	target = entry->d_name;
		if (entry->d_type == DT_DIR)
			target += '/';
		entries.push_back(HTMLPageBuilder::_href(target, uri));
		entry = readdir(directory);
	}
	
	std::sort(entries.begin(), entries.end());
	content += HTMLPageBuilder::_tag("body", HTMLPageBuilder::_unorderedList(entries), true);
	content += "</html>\n";
	
	closedir(directory);
	return (content);
}

const std::string	HTMLPageBuilder::errorPage(const uint16_t errCode)
{
	std::stringstream		ss_errCode; 
	ss_errCode << errCode << " ";
	const std::string		errMessage = ss_errCode.str() + HTTPStatus::getStatusMessage(errCode);
	std::string				content(HTMLPageBuilder::_headerTitled(errMessage));

	content += HTMLPageBuilder::_tag("body", HTMLPageBuilder::_tag("h1", errMessage, false), true);
	content += "</html>\n";
	return (content);
}

// HTML TOOLS
inline const std::string	HTMLPageBuilder::_unorderedList(std::vector<std::string> &v_content)
{
	HTMLPageBuilder::_addTag("li", v_content);
	const std::string	s_content = concatVec(v_content);

	return (HTMLPageBuilder::_tag("ul", s_content, true));
}

inline const std::string	HTMLPageBuilder::_headerTitled(const std::string title)
{
	return ("<html>\n<head><title>" + title + "</title></head>\n");
}

inline const std::string	HTMLPageBuilder::_href(const std::string target, const std::string uri)
{
	return ("<a href=\"" + uri + target + "\">" + target + "</a>");
}

inline void	HTMLPageBuilder::_addTag(const std::string tag, std::vector<std::string> &content)
{
	std::vector<std::string>::iterator	it;

	for (it = content.begin(); it < content.end(); ++it)
		*it = HTMLPageBuilder::_tag(tag, *it, false);
}

inline const std::string	HTMLPageBuilder::_tag(const std::string name, const std::string content, const bool newline)
{
	return ("<" + name + ">" + (newline ? "\n" : "") + content + (newline ? "\n" : "") + "</" + name + ">\n");
}
