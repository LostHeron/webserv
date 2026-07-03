/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualHost.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:24:40 by jweber            #+#    #+#             */
/*   Updated: 2026/07/01 19:14:40 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include "VirtualHost.hpp"
#include "JsonObj.hpp"
#include "config_file.hpp"

VirtualHost::VirtualHost(VirtualHost::s_config const& conf) :
	_conf(conf)
{
	if (this->_conf.name.empty() || this->_conf.root.empty())
		throw std::logic_error("Trying to build a virtual host with empty name and/or empty root : stop trying to setup an host yourself ! Use the intended pipeline !");
}

VirtualHost::VirtualHost(VirtualHost const& other) :
	_conf(other._conf)
{}

VirtualHost::~VirtualHost()
{}

std::vector<std::string> const&	VirtualHost::getName(void) const
{
	return (this->_conf.name);		
}

bool	VirtualHost::InterfaceAllowed(uint32_t interface) const
{
	for (std::vector<VirtualHost::s_ip_range>::const_iterator it = this->_conf.allowedInterface.begin(); it != this->_conf.allowedInterface.end(); ++it)
	{
		if (interface >= it->min && interface <= it->max)
			return (true);
	}
	return (false);
}

std::pair<bool, std::string>	VirtualHost::getError(int err_code) const
{
	std::map<int, std::string>::const_iterator	err_it = this->_conf.error.find(err_code);
	
	if (err_it == this->_conf.error.end())
		return (std::pair<bool, std::string>(false, std::string()));
	return (std::pair<bool, std::string>(true, err_it->second));
}

VirtualHost::UriInfo	VirtualHost::getUriInfo(std::string const& uri) const
{
	std::string										current(uri);
	std::map<std::string, Location>::const_iterator	loc_match_it;
	UriInfo											uri_info(this->_conf);

	if (current[current.size() - 1] != '/')
	{
		loc_match_it = this->_conf.location.find(current);
		if (loc_match_it != this->_conf.location.end())
		{
			VirtualHost::buildUriInfo(uri, *loc_match_it, uri_info);
			return (uri_info);
		}
	}
	while (current.empty() == false)
	{
		loc_match_it = this->_conf.location.find(current);
		if (loc_match_it != this->_conf.location.end())
			break ;
		current.erase(current.end() - 1);
		loc_match_it = this->_conf.location.find(current);
		if (loc_match_it != this->_conf.location.end())
			break ;
		current = current.substr(0, current.find_last_of('/'));
	}
	if (loc_match_it != this->_conf.location.end())
		VirtualHost::buildUriInfo(uri, *loc_match_it, uri_info);
	else
	{
		uri_info._path += uri;
		if (uri_info._uploadPath.empty())
			uri_info._uploadPath = this->_conf.root;
	}
	return (uri_info);
}

void	VirtualHost::buildUriInfo(std::string const& uri, std::pair<std::string, Location> const& loc_pair, UriInfo& uri_info)
{	
	if (loc_pair.second.conf.redirection.empty() == false)
	{
		uri_info._isRedir = true;
		uri_info._path = loc_pair.second.conf.redirection;
		return ;
	}
	if (loc_pair.second.conf.alias.empty() == false)
		uri_info._path = loc_pair.second.conf.alias + uri.substr(loc_pair.first.size());
	else
		uri_info._path += uri;
	if (loc_pair.second.conf.index.empty() == false)
		uri_info._index = loc_pair.second.conf.index;
	if (loc_pair.second.conf.bodySizeInput == true)
		uri_info._maxBodySize = loc_pair.second.conf.maxBodySize;
	if (loc_pair.second.conf.allowedRequest.empty() == false)
		uri_info._allowedRequests = loc_pair.second.conf.allowedRequest;
	uri_info._allowDirList = loc_pair.second.conf.allowDirList;
	uri_info._cgi = loc_pair.second.conf.cgi;
	uri_info._cgiExt = loc_pair.second.conf.cgiExt;
	if (loc_pair.second.conf.uploadPath.empty() == true)
		uri_info._uploadPath = loc_pair.second.conf.alias;
}

VirtualHost::Location::Location(Location::s_config const& conf):
	conf(conf)
{}

VirtualHost::Location::Location(Location const& other) :
	conf(other.conf)
{}

VirtualHost::Location::~Location()
{}

VirtualHost::UriInfo::UriInfo(VirtualHost::s_config conf) :
	_isRedir(false),
	_path(conf.root),
	_index(conf.index),
	_maxBodySize(conf.maxBodySize),
	_allowedRequests(conf.allowedRequest),
	_allowDirList(conf.allowDirList),
	_cgi(conf.cgi),
	_cgiExt(conf.cgiExt),
	_uploadPath(conf.uploadPath)
{}

VirtualHost::UriInfo::UriInfo(UriInfo const& other) :
	_isRedir(other._isRedir),
	_path(other._path),
	_index(other._index),
	_maxBodySize(other._maxBodySize),
	_allowedRequests(other._allowedRequests),
	_allowDirList(other._allowDirList),
	_cgi(other._cgi),
	_cgiExt(other._cgiExt),
	_uploadPath(other._uploadPath)
{}

VirtualHost::UriInfo::~UriInfo()
{}

bool	VirtualHost::UriInfo::isRedir(void)	const
{
	return (this->_isRedir);
}

std::string const&	VirtualHost::UriInfo::getRealPath(void) const
{
	return (this->_path);
}

std::string const&	VirtualHost::UriInfo::getIndex(void) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	return (this->_index);
}

uint64_t	VirtualHost::UriInfo::getBodySize(void) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	return (this->_maxBodySize);
}

bool	VirtualHost::UriInfo::isRequestAllowed(std::string const& req) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	for (std::vector<std::string>::const_iterator it = this->_allowedRequests.begin(); it != this->_allowedRequests.end(); ++it)
	{
		if (*it == req)
			return (true);
	}
	return (false);
}

bool	VirtualHost::UriInfo::isDirListAllowed(void) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	return (this->_allowDirList);
}

bool	VirtualHost::UriInfo::isCgiAllowed(void) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	return (this->_cgi);
}

bool	VirtualHost::UriInfo::isCgiExtAllowed(std::string const& cgi_ext) const
{
	if (this->_isRedir)
		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
	for (std::vector<std::string>::const_iterator it = this->_cgiExt.begin(); it != this->_cgiExt.end(); ++it)
	{
		if (*it == cgi_ext)
			return (true);
	}
	return (false);
}

//<<<<<<< HEAD
std::string						VirtualHost::buildSessionId(void)
{
	static const std::string	alphanum = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string					id;
	std::srand(time(0));

	for (unsigned int i = 0; i < SESSION_ID_CHAR; ++i)
		id += alphanum[rand() % (alphanum.length() - 1)];
	return (id);
}

bool						VirtualHost::isIdAvailable(std::string &id)
{
	std::map<std::string, Session>::iterator iter;
	for (iter = this->_sessions.begin(); iter != this->_sessions.end(); ++iter)
	{
		if (id == iter->second.getSessionId())
			return (false);
	}
	return (true);
}

std::map<std::string, Session>	&VirtualHost::getSessions(void)
{
	return (this->_sessions);
}

void							VirtualHost::addSession(Session &session)
{
	std::string sessionId = session.getSessionId();
	if (sessionId != "")
		this->_sessions[sessionId] = session;
}

void							VirtualHost::removeSession(const std::string &sessionId)
{
	if (sessionId != "")
		this->_sessions.erase(sessionId);
}

void							VirtualHost::removeOldSessions(void)
{
	const time_t								now = time(NULL);
	std::map<std::string, Session>::iterator	iter;
	for (iter = this->_sessions.begin(); iter != this->_sessions.end(); ++iter)
	{
		Cookie			&sessionIdCookie = iter->second.getSessionCookies()[Cookie::permanentCookies[Cookie::SESSION]];
		std::string		sessionId = iter->first;
		if (std::difftime(now, sessionIdCookie.getInitializationDate()) >= sessionIdCookie.getMaxAge())
			this->removeSession(sessionId);
	}
}

void							VirtualHost::updateSession(const std::string &id, std::map<std::string, Cookie> &cookies)
{
	Session	&session = this->_sessions[id];

	session.deleteOldCookies();

	std::map<std::string, Cookie>::iterator	iter;
	for (iter = cookies.begin(); iter != cookies.end(); ++iter)
	{
		session.updateCookie(iter->second);
	}

//=======
//std::string const&	VirtualHost::UriInfo::getUploadPath(void) const
//{
//	if (this->_isRedir)
//		throw std::logic_error("This uri is a redirection to another path : the requested info is non-existant");
//	return (this->_uploadPath);
//>>>>>>> config_file
}
