/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/06/08 17:24:36 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "IntSchema.hpp"

IntSchema::IntSchema(std::string const& name, bool is_required, bool allow_multiple) :
	ASchema(name, JsonObj::INT, is_required, allow_multiple),
	_min(std::numeric_limits<int>::min()),
	_max(std::numeric_limits<int>::max())
{}

IntSchema::~IntSchema()
{}

void	IntSchema::setMin(int min)
{
	this->_min = min;
}

void	IntSchema::setMax(int max)
{
	this->_max = max;
}

void	IntSchema::checkValue(JsonObj const& object) const
{
	if (object.getVal<int>() < this->_min || object.getVal<int>() > this->_max)
		throw std::invalid_argument("Object '" + this->getName() + "' has its value out of the expected range");
}