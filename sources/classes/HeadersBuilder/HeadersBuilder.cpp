/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeadersBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:37:04 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 17:00:56 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeadersBuilder.hpp"
#include "HTTPStatus.hpp"
#include "HTTPStatus.cpp" // hm strange
#include "default_pages.hpp"
#include <sstream>

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

HeadersBuilder& HeadersBuilder::buildCRLF()
{
	this->response.append("\r\n");
	return (*this);
}

static std::string default_error_page(int errorCode);

HeadersBuilder&	HeadersBuilder::buildBody(int errorCode)
{
	this->response.append(default_error_page(errorCode));
	return (*this);
}

HeadersBuilder&	HeadersBuilder::buildBody(const std::string& content)
{
	this->response.append(content);
	return (*this);
}

static std::string default_error_page(int errorCode)
{
	switch (errorCode) {
		//case 200: return "OK";
		case 400: return ERROR_PAGE_400;
		//case 404: return ERROR_PAGE_404;
		default: return "";
	}
}

std::string	HeadersBuilder::build()
{
	return (this->response);
}

