/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GETReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abetemps <abetemps@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:44:37 by abetemps          #+#    #+#             */
/*   Updated: 2026/06/26 19:35:53 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTMLPageBuilder.hpp"
#include "GETReq.hpp"
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

// Constructors/Destructor =====================================================
GETReq::GETReq(const ARequest &src):
	ARequest(src) {}

GETReq::GETReq(const GETReq &cpy):
	ARequest(cpy) {}

GETReq::~GETReq(void) {}

// Member functions ============================================================
void	GETReq::_execute(Response &resp, const VirtualHost::UriInfo &uriInfo)
{
	uint16_t						&status = resp.getStatus();
	std::pair<int, std::string>		&resource = resp.getResource();
	std::string						&content = resp.getContent();

	DIR	*directory = this->_tryOpenDirectory(resource.second.c_str());
	if (directory && uriInfo.getIndex() != "")
	{
		this->_uri += uriInfo.getIndex() + "/";
		resource.second += "/" + uriInfo.getIndex();
		if (!resp.isCGI())
			resp.setCGI(uriInfo.isCgiExtAllowed(this->_getFileExtension(this->_uri)));

		closedir(directory);
		directory = this->_tryOpenDirectory(resource.second.c_str());
	}
	
	if (directory)
	{
		if (!uriInfo.isDirListAllowed())
		{
			closedir(directory);
			status = HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN;
			return;
		}
		content = HTMLPageBuilder::dirListingPage(directory, this->_uri);
		return;
	}
	else
	{
		switch (errno)
		{
			case (ENOTDIR):
				if (resp.isCGI())
					break;
				if ((resource.first = this->_tryOpenFile(resource.second.c_str())) >= 0)
					break;
				__attribute__((fallthrough));
			case (EACCES):
				status = HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN;
				break;
			case (ENOENT):
				status = HTTPStatus::C_ERR + HTTPStatus::NOT_FOUND;
				break;
			default:
				if (!resp.isCGI())
					status = HTTPStatus::S_ERR + HTTPStatus::INTERNAL;
				break;
		}
	}
<<<<<<< HEAD
	return (status);
}

Response	GETReq::execute(void)
{
	const VirtualHost::UriInfo		&uriInfo = this->_vhost.getUriInfo(this->_uri);
	Response						resp(this->_fd, uriInfo.isCgiAllowed());

	resp.setRedir(uriInfo.isRedir());
	// if redir, set to RAW path (config file)
	resp.setResourcePath(uriInfo.getRealPath());

	const std::vector<Cookie> receivedCookies(this->_headerToCookie());

	std::vector<Cookie>::const_iterator it;
	for (it = receivedCookies.begin(); it != receivedCookies.end(); ++it)
	{
		// Cookie::checkLifetime(*it);	(static void	Cookie::checkLifetime(Cookie &cookie))
		// Cookie::checkID(*it);		(static void	Cookie::checkSession(Cookie &cookie))
	}
	resp.setCookies(this->_headerToCookie());


# ifdef	DEBUG
	for (it = resp.getCookies().begin(); it != resp.getCookies().end(); ++it)
		std::cout << *it << std::endl;
#endif


	if (!uriInfo.isRequestAllowed(this->_method))
		resp.setStatus(HTTPStatus::C_ERR + HTTPStatus::FORBIDDEN);
	else
		resp.setStatus(this->_fetchResource(resp.getResource(), resp.getContent(), uriInfo, resp.isCGI()));

	if (resp.getStatus() >= HTTPStatus::C_ERR)
		resp.error(this->_vhost);

	// test cookies

	std::vector<Cookie> cookies;
	Cookie cookie(Cookie::kvPair(Cookie::permanentCookies[Cookie::SESSION], "98ef"));
	cookie.setDomain("localhost");
	cookie.setPath(this->_uri);
	cookie.setHttpOnly(false);
	cookie.setSecure(true);
	cookie.setSameSite(Cookie::LAX);
	cookies.push_back(cookie);

	cookie = Cookie(Cookie::kvPair(Cookie::permanentCookies[Cookie::THEME], THEME_COOKIE_DEFAULT));
	cookie.setDomain("localhost");
	cookie.setPath(this->_uri);
	cookie.setHttpOnly(true);
	cookie.setSecure(true);
	cookie.setSameSite(Cookie::NONE);
	cookies.push_back(cookie);

	resp.setCookies(cookies);

	// test cookies

	return (resp);
=======
>>>>>>> 34057b22f21317429711790a992f2b447cf55bfb
}
