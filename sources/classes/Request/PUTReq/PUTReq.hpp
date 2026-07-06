/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PUTReq.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:47 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 13:43:09 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		__PUTREQ_HPP__
# define	__PUTREQ_HPP__

# include <fstream>
# include "ARequest.hpp"

class	PUTReq: public ARequest
{
	public:
		PUTReq(const ARequest &cpy);
		PUTReq(const PUTReq &cpy);
		~PUTReq(void);

		uint16_t						appendBodyToFile(const std::string &body);

	private:
		static std::vector<std::string>	_lockedFiles;
		static bool						_lockFile(const std::string &file);
		static void						_unlockFile(const std::string &file);

		static bool						_isFileLocked(const std::string &file);

		void							_execute(Response &resp, const VirtualHost::UriInfo &uriInfo);
		void							_openPath(Response &resp);

		std::ofstream					_file;
		std::string						_filePath;

};

#endif
