/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/22 17:18:43 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"
# include "HTMLPageBuilder.hpp"
# include <unistd.h>
# include <fcntl.h>

// Construction/Destruction ====================================================
Response::Response(const int fd, bool isCGI):
	AMessage(fd),
	_status(HTTPStatus::SUCCESS + HTTPStatus::OK),
	_resource(std::pair<int, std::string>(-1, "")),
	_content(""),
	_cookies(),
	_cgi(isCGI),
	_pathInfo(""),
	_redir(false) {}

Response::Response(uint16_t errCode, const VirtualHost &vHost):
	AMessage(-1),
	_status(errCode)
	// cookies ???
{
	this->error(vHost);
}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resource(cpy._resource),
	_content(cpy._content),
	_cookies(cpy._cookies),
	_cgi(cpy._cgi),
	_pathInfo(cpy._pathInfo),
	_redir(cpy._redir) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign)
{
	if (this != &assign)
	{
		this->_status = assign._status;
		this->_resource = assign._resource;
		this->_content = assign._content;
		this->_cookies = assign._cookies;
		this->_cgi = assign._cgi;
		this->_redir = assign._redir;
	}
	return (*this);
}

// Setters =====================================================================
void	Response::setStatus(const uint16_t status)
{
	this->_status = status;
}

void	Response::setCookies(std::map<std::string, Cookie> cookies)
{
	this->_cookies = cookies;
}

void	Response::setResource(std::pair<int, std::string> &resource)
{
	this->_resource = resource;
}

void	Response::setResourceFd(int fd)
{
	this->_resource.first = fd;
}

void	Response::setResourcePath(std::string path)
{
	this->_resource.second = path;
}

void	Response::setContent(const std::string &content)
{
	this->_content = content;
}

void	Response::setCGI(const bool isCGI)
{
	this->_cgi = isCGI;
}

void	Response::setPathInfo(const std::string &pathInfo)
{
	this->_pathInfo = pathInfo;
}

void	Response::setRedir(const bool isRedir)
{
	this->_redir = isRedir;
}

// Getters =====================================================================
uint16_t						&Response::getStatus(void)				{	return (this->_status);		}
std::map<std::string, Cookie>	&Response::getCookies(void)				{	return (this->_cookies);	}
std::pair<int, std::string>		&Response::getResource(void)			{	return (this->_resource);	}
std::string						&Response::getContent(void)				{ 	return (this->_content);	}
bool							&Response::isCGI(void)					{	return (this->_cgi);		}
std::string						&Response::getPathInfo(void)			{	return (this->_pathInfo);	}
bool							Response::isRedir(void)			const	{	return (this->_redir);		}


// Member functions ============================================================
void									Response::error(const VirtualHost &vHost)
{
	// check Vhost for error page
	const std::pair<bool, std::string>	errPage = vHost.getError(this->_status);
	if (errPage.first)
	{
		this->_resource.first = open(errPage.second.c_str(), O_RDONLY);
		if (this->_resource.first != -1)
		{
			this->_resource.second = errPage.second;
			this->_content = "";
			return;
		}
	}
	this->_resource.first = -1;
	this->_resource.second = "";
	this->_content = HTMLPageBuilder::errorPage(this->_status);
}

