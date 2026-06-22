/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/01 19:49:05 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__GETREQ_HPP__
# define	__GETREQ_HPP__

# include "ARequest.hpp"
# include <dirent.h>
# include <errno.h>
# include <sys/types.h>

class	GETReq: public ARequest // only GET ?
{
	public:
		// GETReq(void);
		// GETReq(const std::string &type, const std::string &header, const std::string &body);
		GETReq(const ARequest &src);
		GETReq(const GETReq &cpy);
		~GETReq(void);

		// GETReq		&operator=(const GETReq &assign);

		Response		execute(void);

	private:
		uint16_t	_fetchResource(std::pair<int, std::string> &resource, std::string &content, const VirtualHost::UriInfo &uriInfo);
		int			_dirListing(DIR *dir) const;
		DIR			*_tryOpenDirectory(const char *path) const;
		int			_tryOpenFile(const char *path) const;
};

#endif
