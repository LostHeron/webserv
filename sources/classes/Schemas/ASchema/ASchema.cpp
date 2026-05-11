/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASchema.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 20:36:35 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 15:44:29 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <stdexcept>
#include "JsonObj.hpp"
#include "ASchema.hpp"

ASchema::ASchema(std::string const& name, JsonObj::e_jsonType type, bool is_required, bool allow_multiple) :
	_name(name),
	_type(type),
	_isRequired(is_required),
	_allowMultiple(allow_multiple)
{}

ASchema::~ASchema()
{}

void	ASchema::validate(std::map<std::string, JsonObj> const& obj_map) const
{
	std::map<std::string, JsonObj>::const_iterator obj_it = obj_map.find(this->_name);

	if (obj_it == obj_map.end())
		throw std::logic_error("Object '" + this->_name + "' is required and missing");
	if (obj_it->second.getType() == JsonObj::ARRAY && this->_allowMultiple == true)
	{
		for (std::vector<JsonObj>::const_iterator arr_it = obj_it->second.getArray().begin(); arr_it != obj_it->second.getArray().end(); ++arr_it)
		{
			this->checkType(*arr_it);
			this->checkValue(*arr_it);
		}
		return ;
	}
	this->checkType(obj_it->second);
	this->checkValue(obj_it->second);
}

void	ASchema::checkType(JsonObj const& object) const
{
	if (object.getType() == this->_type)
		throw std::domain_error("Object '" + this->_name + "' doesn't have the right type");
}

std::string const&	ASchema::getName(void) const
{
	return (this->_name);
}
