/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 15:49:16 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "IntSchema.hpp"

IntSchema::IntSchema(std::string const& name, bool is_required, bool is_unique) :
	ASchema(name, JsonObj::INT, is_required, is_unique)
{
	this->_min = std::numeric_limits<int>::min();
	this->_max = std::numeric_limits<int>::max();
}

IntSchema::~IntSchema()
{}

void	IntSchema::checkValue(JsonObj const& object) const
{
	if (object.getInt() < this->_min || object.getInt() > this->_max)
		throw std::invalid_argument("Object '" + this->getName() + "' has its value out of the expected range");
}