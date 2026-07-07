/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PUTReq.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/04/13 17:38:59 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PUTReq.hpp"
#include <vector>
#include <sys/stat.h>

# define TMP_PUT_PATH "/home/abetemps/temp_webserv_root/put/"

// Constructors/Destructor =====================================================
PUTReq::PUTReq(const ARequest &cpy):
	ARequest(cpy) {}

PUTReq::PUTReq(const PUTReq &cpy):
	ARequest(cpy) {}

PUTReq::~PUTReq(void)
{
	PUTReq::_unlockFile(this->_filePath);
}

// Static members ==============================================================
std::vector<std::string>		PUTReq::_lockedFiles;

bool							PUTReq::_isFileLocked(const std::string &file)
{
	std::vector<std::string>::iterator	iter;	

	for (iter = PUTReq::_lockedFiles.begin(); iter < PUTReq::_lockedFiles.end(); ++iter)
		if (file == *iter)
			return (true);
	return (false);
}

bool							PUTReq::_lockFile(const std::string &file)
{
	if (PUTReq::_isFileLocked(file))
		return (false);
	PUTReq::_lockedFiles.push_back(file);
	return (true);
}

void							PUTReq::_unlockFile(const std::string &file)
{
	std::vector<std::string>::iterator	iter;	

	for (iter = PUTReq::_lockedFiles.begin(); iter < PUTReq::_lockedFiles.end(); ++iter)
		if (file == *iter)
			PUTReq::_lockedFiles.erase(iter);
}


// Member functions ============================================================
uint16_t	PUTReq::appendBodyToFile(const std::string &body)
{
	this->_file << body;
	if (!this->_file.good())
	{
		switch (errno)
		{
			case(EACCES):
				return (HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
			case(ENOENT):
				return (HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND);
			default:
				return (HTTPStatus::S_ERR + HTTPStatus::INTERNAL);
		}
	}
	return (0);
}

void		PUTReq::_openPath(Response &resp)
{
	struct stat st;

	if (stat(this->_filePath.c_str(), &st) == 0)
		resp.setStatus(HTTPStatus::SUCCESS + HTTPStatus::NO_CONTENT);
	else
		resp.setStatus(HTTPStatus::SUCCESS + HTTPStatus::CREATED);

	this->_file.open(this->_filePath.c_str());
	if (!this->_file.is_open())
	{
		switch (errno)
		{
			case(EACCES):
				resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
				return;	
			case(ENOENT):
				resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND);
				return;	
			default:
				resp.setStatus(HTTPStatus::S_ERR + HTTPStatus::INTERNAL);
				return;
		}
	}

}

void		PUTReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	this->_filePath = uriInfo.getRealPath(this->_uri);
	if (PUTReq::_lockFile(this->_filePath))
	{
		this->_openPath(resp);
	}
	else
	{
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::CONFLICT);
	}
}
