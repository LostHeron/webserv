/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoolSchema.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:47:44 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 18:32:43 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <limits>
#include "JsonObj.hpp"
#include "BoolSchema.hpp"

BoolSchema::BoolSchema(std::string const& name, bool is_required, bool allow_multiple) :
	ASchema(name, JsonObj::BOOL, is_required, allow_multiple)
{}

BoolSchema::~BoolSchema()
{}

void    BoolSchema::checkValue(JsonObj const& object) const
{}