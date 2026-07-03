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
#include <fstream>
#include <vector>

# define TMP_PUT_PATH "/home/abetemps/temp_webserv_root/put/"

// Constructors/Destructor =====================================================
PUTReq::PUTReq(const ARequest &cpy):
	ARequest(cpy) {}

PUTReq::PUTReq(const PUTReq &cpy):
	ARequest(cpy) {}

PUTReq::~PUTReq(void)
{
	std::string	upPath = TMP_PUT_PATH + this->_uri;//uriInfo.getRealPath(this->_uri);
	PUTReq::_unlockFile(upPath);
}

// Static members ==============================================================
std::vector<std::string>		PUTReq::_lockedFiles;

bool							PUTReq::_isFileLocked(const std::string &file)
{
	std::vector<std::string>::iterator	iter;	

	for (iter = PUTReq::_lockedFiles.begin(); iter < PUTReq::_lockedFiles.end(); ++iter)
	{
		std::cout << "lockedFile: " << *iter << std::endl;
		if (file == *iter)
		{
			std::cout << "FOUND!" << std::endl;
			return (true);
		}
	}
	return (false);
}

bool							PUTReq::_lockFile(const std::string &file)
{
	if (PUTReq::_isFileLocked(file))
	{
		return (false);
	}
	PUTReq::_lockedFiles.push_back(file);
	return (true);
}

void							PUTReq::_unlockFile(const std::string &file)
{
	std::vector<std::string>::iterator	iter;	

	for (iter = PUTReq::_lockedFiles.begin(); iter < PUTReq::_lockedFiles.end(); ++iter)
	{
		std::cout << "lockedFile: " << *iter << std::endl;
		if (file == *iter)
		{
			PUTReq::_lockedFiles.erase(iter);
			std::cout << "DELETING!" << std::endl;
		}
	}
}


// Member functions ============================================================
void	PUTReq::_uploadFile(Response &resp, const std::string &upPath)
{
	// if file is being created, resp.setStatus(HTTPStatus::SUCCES + HTTPStatus::CREATED);
	// else resp.setStatus(HTTPStatus::SUCCES + HTTPStatus::OK);

	std::ofstream	of;
	of.open(upPath.c_str());
	if (!of.is_open())
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
	// of << this->_body;
	of << "This body" ;
}

void	PUTReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	(void) uriInfo;

	std::string	upPath = TMP_PUT_PATH + this->_uri;//uriInfo.getRealPath(this->_uri);
	if (PUTReq::_lockFile(upPath))
	{
		this->_uploadFile(resp, upPath);
	}
	else
	{
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::CONFLICT);
	}
}
