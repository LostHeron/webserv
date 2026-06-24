/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:37:04 by jweber            #+#    #+#             */
/*   Updated: 2026/06/22 17:59:31 by abetemps         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include "HTTPStatus.hpp"
#include "HTTPStatus.cpp" // hm strange
#include "default_pages.hpp"
#include <locale>
#include <sstream>
#include <vector>

HeadersBuilder::HeadersBuilder(){};
HeadersBuilder::~HeadersBuilder(){};

HeadersBuilder& HeadersBuilder::initialize()
{
	this->response.clear();
	this->response.reserve(4096);
	return (*this);
}

HeadersBuilder& HeadersBuilder::buildStatusLine(const std::string& version, int errorCode)
{
	this->response.append(version);
	this->response.append(" ");

	std::stringstream	ss;
	std::string			errorCode_str;
	ss << errorCode;
	ss >> errorCode_str;
	this->response.append(errorCode_str);
	this->response.append(" ");
	
	std::string test = HTTPStatus::getStatusMessage(errorCode);
	this->response.append(test);
	this->response.append("\r\n");
	return (*this);
}

HeadersBuilder& HeadersBuilder::buildDate()
{
	this->response.append("Date: ");
	std::time_t timep = time(NULL);
	struct tm *t = std::gmtime(&timep);
	char buf[1024];
	strftime(buf, 1024, "%a, %d %b %Y %T UTC", t);
	this->response.append(buf, strlen(buf));
	this->response.append("\r\n");
	return (*this);
}

HeadersBuilder& HeadersBuilder::buildHeaderKeyValue(const std::string& key, const std::string& value)
{
	this->response.append(key);
	this->response.append(": ");
	this->response.append(value);
	this->response.append("\r\n");
	return (*this);
}

HeadersBuilder& HeadersBuilder::buildHeaderKeyVecValue(const std::string& key, const std::vector<std::string>& values)
{
	this->response.append(key);
	this->response.append(": ");
	for (size_t i = 0; i < values.size(); i++)
	{
		if (i != 0)
			this->response.append(", ");
		this->response.append(values.at(i));
	}
	this->response.append("\r\n");
	return (*this);
}

HeadersBuilder& HeadersBuilder::buildCRLF()
{
	this->response.append("\r\n");
	return (*this);
}

HeadersBuilder&	HeadersBuilder::buildBody(const std::string& content)
{
	this->response.append(content);
	return (*this);
}

HeadersBuilder&	HeadersBuilder::buildCookies(const std::vector<Cookie> &cookies)
{
	if (cookies.empty())
		return (*this);

	std::vector<Cookie>::const_iterator	it;

	for (it = cookies.begin(); it != cookies.end(); ++it)
		this->response.append(it->cookieToStr());
	return (*this);
}

std::string	HeadersBuilder::build()
{
	return (this->response);
}

