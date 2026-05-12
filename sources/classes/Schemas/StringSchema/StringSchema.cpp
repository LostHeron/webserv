/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringSchema.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 19:17:40 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "StringSchema.hpp"

StringSchema::StringSchema(std::string const& name, bool is_required, bool allow_multiple) :
	ASchema(name, JsonObj::STRING, is_required, allow_multiple),
	_validator(NULL)
{}

StringSchema::~StringSchema()
{}

void	StringSchema::addValidator(void (*validator)(void))
{
	this->_validator = validator;
}

void	StringSchema::checkValue(JsonObj const& object) const
{
	if (this->_validator != NULL)
		this->_validator();
}