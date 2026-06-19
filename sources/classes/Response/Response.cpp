/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:31:13 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/05 15:21:27 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"
# include "HTMLPageBuilder.hpp"

// Construction/Destruction ====================================================
Response::Response(const int fd):
	AMessage(fd),
	_status(HTTPStatus::SUCCESS + HTTPStatus::OK),
	_resource(std::pair<int, std::string>(-1, "")),
	_content() {}

Response::Response(uint16_t errCode, const VirtualHost &vHost):
	AMessage(-1),
	_status(errCode)
{
	this->error(vHost);
}

Response::Response(const Response &cpy):
	AMessage(cpy._fd),
	_status(cpy._status),
	_resource(cpy._resource),
	_content(cpy._content) {}

Response::~Response(void) {}


// Ops overloading =============================================================
Response			&Response::operator=(const Response &assign){ (void) assign; return (*this); }

// Setters =====================================================================
void	Response::setStatus(const uint16_t status)
{
	this->_status = status;
}

void	Response::setResource(std::pair<int, std::string> &resource)
{
	this->_resource = resource;
}

void	Response::setResourceFd(int fd)
{
	this->_resource.first = fd;
}

void	Response::setResourcePath(std::string &path)
{
	this->_resource.second = path;
}

void	Response::setContent(const std::string &content)
{
	this->_content = content;
}

// Getters =====================================================================
const uint16_t					&Response::getStatus(void)		const	{	return (this->_status);		}
std::pair<int, std::string>		&Response::getResource(void)			{	return (this->_resource);	}
std::string						&Response::getContent(void)				{ 	return (this->_content);	}


// Member functions ============================================================
void									Response::error(const VirtualHost &vHost)
{
	(void) vHost;
	// check Vhost for error page
	// if (const std::string errPage = vhost.errorPageExist(this->_status))
	// {
	// 	this->_resource.first = open(errPage.c_str(), O_RDONLY);
	// 	this->_resource.second = errPage;
	//	this->_content = "";
	// }
	// else
	// {
	this->_resource.first = -1;
	this->_resource.second = "";
	this->_content = HTMLPageBuilder::errorPage(this->_status);
	// }
	
}

