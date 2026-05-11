/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringSchema.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 14:59:13 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "StringSchema.hpp"

StringSchema::StringSchema(std::string const& name, bool is_required, bool is_unique) :
	ASchema(name, JsonObj::STRING, is_required, is_unique)
{}

StringSchema::~StringSchema()
{}

bool	StringSchema::checkValue(JsonObj const& object)
{
	return (true);
}