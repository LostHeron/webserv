/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjSchema.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 18:35:03 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include "JsonObj.hpp"
#include "ObjSchema.hpp"

ObjSchema::ObjSchema(std::string const& name, bool is_required, bool allow_multiple) :
	ASchema(name, JsonObj::SUBOBJ, is_required, allow_multiple),
	_fields()
{}

ObjSchema::~ObjSchema()
{
	for (std::vector<ASchema*>::iterator field_it = this->_fields.begin(); field_it != this->_fields.end(); ++field_it)
	{
		delete *field_it;
	}
}

void	ObjSchema::addField(ASchema* schema)
{
	this->_fields.push_back(schema);
}

void	ObjSchema::checkValue(JsonObj const& object) const
{
	for (std::vector<ASchema*>::const_iterator field_it = this->_fields.begin(); field_it != this->_fields.end(); ++field_it)
	{
		(*field_it)->validate(object.getSubObj());
	}
}