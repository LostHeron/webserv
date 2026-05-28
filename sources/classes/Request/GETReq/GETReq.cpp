/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:39:13 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GETReq.hpp"
#include <cstring>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>

// Constructors/Destructor =====================================================
// GETReq::GETReq(void):
// 	ARequest(type, header, body) {}

GETReq::GETReq(const ARequest &cpy):
	ARequest(cpy) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================

// tempp!!!!
void send_file(int fd, int client)
{
	int rv = 0;
	char buf[1024 + 1];
	do
	{
		buf[rv] = '\0';
		write(client, buf, rv);
		rv = read(fd, buf, 1024);
	} while (rv);
}
// tempp!!!!

int	GETReq::_displayDir(DIR *dir) const
{
	const std::string htmlHeader = "<html>\n<head><title>Index of /</title></head>\n<body>\n<h1>Index of /</h1><hr><pre>\n";
	const std::string htmlFooter = "</pre><hr></body>\n</html>";
	const std::string htmlHrefStart = "<a href=\"";
	const std::string htmlHrefEnd = "\">../</a>\n";
	struct dirent	*entry = readdir(dir);
	int				fds[2];

	if (pipe(fds))
		return (-1);

	write(fds[1], htmlHeader.c_str(), htmlHeader.length());
	while (entry)
	{
		write(fds[1], htmlHrefStart.c_str(), htmlHrefStart.length());
		write(fds[1], entry->d_name, std::strlen(entry->d_name));
		write(fds[1], htmlHrefEnd.c_str(), htmlHrefEnd.length());
		entry = readdir(dir);
	}

	write(fds[1], htmlFooter.c_str(), htmlFooter.length());

	closedir(dir);
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
	uint16_t	status = SUCCESS + OK;

	// TEMP DEBUG
	const std::string path = TEMP_ROOT + this->_uri;
	std::cout	<< "URI to fetch: " << this->_uri 
		<< " for full path: " << path
		<< std::endl;
	// TEMP DEBUG

	// get location AND permission = config_file
	// COCO: std::pair<std::string real_path, bool allowed> getPathAndPermission(std::string method, std::string uri);
	// path = pair.first;
	// perm = pair.second;

	// check dir or file, if dir return index page
	DIR	*pathEntry = this->_tryOpenDirectory(path.c_str());
	if (pathEntry)	
		resourceFd = this->_displayDir(pathEntry);
	else
	{
		// std::cout << "ERRNO IS::::::   " << errno << std::endl;
		switch (errno)
		{
			case (ENOTDIR):
				if ((resourceFd = this->_tryOpenFile(path.c_str())) >= 0)
					break;
			case (EACCES):
				status = C_ERR + FORBIDDEN;
				break;
			case (ENOENT):
				status = C_ERR + NOT_FOUND;
				break;
		}
	}

	// metadata settings
	resp.setResourceFd(resourceFd);
	resp.setStatus(status);

	// DEBUG
	send_file(resourceFd, this->_fd);
	// DEBUG

	return (resp);
}
