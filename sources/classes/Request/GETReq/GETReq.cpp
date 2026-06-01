/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/05/31 17:14:34 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPStatus.hpp"
#include "GETReq.hpp"
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

// Constructors/Destructor =====================================================
// GETReq::GETReq(void):
// 	ARequest(type, header, body) {}

GETReq::GETReq(const ARequest &cpy):
	ARequest(cpy) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
int	GETReq::_dirListing(DIR *dir) const
{
	const std::string 	htmlHeader = "<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n";
	const std::string 	htmlFooter = "</pre><hr></body>\n</html>";
	const std::string 	htmlHrefStart = "<a href=\"";
	const std::string 	htmlHrefMid = "\">";
	const std::string 	htmlHrefEnd =	"</a>\n";
	struct dirent		*entry = readdir(dir);
	int					fds[2];

	if (pipe(fds))
		return (-1);

	write(fds[1], htmlHeader.c_str(), htmlHeader.length());
	while (entry)
	{

		write(fds[1], htmlHrefStart.c_str(), htmlHrefStart.length());
		write(fds[1], entry->d_name, std::strlen(entry->d_name));
		// to add following line if entry is a directory
		// write(fds[1], "/", 1);
		write(fds[1], htmlHrefMid.c_str(), htmlHrefMid.length());
		write(fds[1], entry->d_name, std::strlen(entry->d_name));
		// to add following line if entry is a directory
		// write(fds[1], "/", 1);
		write(fds[1], htmlHrefEnd.c_str(), htmlHrefEnd.length());
		entry = readdir(dir);
	}

	write(fds[1], htmlFooter.c_str(), htmlFooter.length());

	closedir(dir);
	close(fds[1]);
	return (fds[0]);
}

DIR	*GETReq::_tryOpenDirectory(const char *path) const
{
	DIR	*dir = opendir(path);

	if (dir)
		return (dir);
	return (NULL);
}

int	GETReq::_tryOpenFile(const char *path) const
{
	int	fd = open(path, O_RDONLY);

	return (fd);
}

Response	GETReq::execute(void)
{
	Response	resp(this->_fd);
	int			resourceFd = -1;
	uint16_t	status = HTTPStatus::SUCCESS + HTTPStatus::OK;

	// TEMP DEBUG
	std::pair<std::string, bool> permission = _vhost.getPathReq(this->_uri, this->_method);
	const std::string path = permission.first;
	std::cout << "URI to fetch: " << this->_uri 
		<< " for real path: " << path
		<< " for method: " << this->_method 
		<< (permission.second ? " <ALLOWED>" : " <FORBIDEN>")
		<< std::endl;
	// TEMP DEBUG
	
	// check dir or file, if dir return index page
	DIR	*pathEntry = this->_tryOpenDirectory(path.c_str());
	if (pathEntry)	
		resourceFd = this->_dirListing(pathEntry);
	else
	{
		// std::cout << "ERRNO IS::::::   " << errno << std::endl;
		switch (errno)
		{
			case (ENOTDIR):
				if ((resourceFd = this->_tryOpenFile(path.c_str())) >= 0)
					break;
				break;
			case (EACCES):
				status = HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN;
				break;
			case (ENOENT):
				status = HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND;
				break;
		}
	}

	// metadata settings
	resp.setResourceFd(resourceFd);
	resp.setStatus(status);

	return (resp);
}
