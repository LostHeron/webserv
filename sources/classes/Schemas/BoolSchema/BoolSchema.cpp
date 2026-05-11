/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoolSchema.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 14:57:14 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "BoolSchema.hpp"

BoolSchema::BoolSchema(std::string const& name, bool is_required, bool is_unique) :
	ASchema(name, JsonObj::BOOL, is_required, is_unique)
{}

BoolSchema::~BoolSchema()
{}

bool    BoolSchema::checkValue(JsonObj const& object)
{
	return (true);
}