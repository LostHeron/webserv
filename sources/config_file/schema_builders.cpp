/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema_builders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:22:01 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 19:47:52 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ObjSchema.hpp"
#include "IntSchema.hpp"
#include "BoolSchema.hpp"
#include "StringSchema.hpp"

void   server_schema_builder(ObjSchema& server_schema)
{   
    server_schema.addField(new IntSchema("listen", true, true));
    // set min and max between 1 and 2^16 - 1
    server_schema.addField(new StringSchema("name", true, true));
    server_schema.addField(new StringSchema("root", true, false));
    server_schema.addField(new StringSchema("index", false, false));
    server_schema.addField(new IntSchema("max body", false, false));
    // set min to 0 and max to whatever
    server_schema.addField(new StringSchema("interface", false, true));
    // set validator for ip addresses and ranges of ip addresses
    server_schema.addField(new StringSchema("allowed request", false, true));
    server_schema.addField(new ObjSchema("error page", false, true));
    // set schema for error pages
    server_schema.addField(new ObjSchema("location", false, true));
    // set schema for locations
}
