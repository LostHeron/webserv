/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 19:45:21 by cviel            ###   ########.fr       */
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

IntSchema::IntSchema(IntSchema const& other) :
	ASchema(other),
	_min(other._min),
	_max(other._max)
{}

IntSchema::~IntSchema()
{}

IntSchema&	IntSchema::operator=(IntSchema const& other)
{
	if (this != &src)
	{
		ASchema::this = ASchema::other;
		this->_min = other._min;
		this->_max = other._max;
	}
	return (*this);
}

bool	IntSchema::checkValue(JsonObj const& object)
{
	return (object.getInt() >= this->_min && object.getInt() <= this->_max);
}