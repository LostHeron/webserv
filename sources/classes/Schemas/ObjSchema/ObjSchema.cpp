/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjSchema.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 19:45:28 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "ObjSchema.hpp"

ObjSchema::ObjSchema(std::string const& name, bool is_required, bool is_unique) :
	ASchema(name, JsonObj::SUBOBJ, is_required, is_unique),
	_fields()
{}

ObjSchema::ObjSchema(ObjSchema const& other) :
	ASchema(other),
	_fields(other._fields)
{}

ObjSchema::~ObjSchema()
{}

ObjSchema&	ObjSchema::operator=(ObjSchema const& other)
{
	if (this != &src)
	{
		ASchema::this = ASchema::other;
		this->_fields = other._fields;
	}
	return (*this);
}

bool	ObjSchema::checkValue(JsonObj const& object)
{
	for (std::vector<ASchema*>::const_iterator field_it = this->_fields.begin(); field_it != this->_fields.end(); ++field_it)
	{
		if (field_it->validate(object.getSubObj()) == false)
			return (false);
	}
	return (true);
}