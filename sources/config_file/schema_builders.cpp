/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema_builders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:22:01 by cviel             #+#    #+#             */
/*   Updated: 2026/05/13 17:18:27 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_file.hpp"
#include "ObjSchema.hpp"
#include "IntSchema.hpp"
#include "BoolSchema.hpp"
#include "StringSchema.hpp"

static void	error_page_schema_builder(ObjSchema& error_page_schema);
static void	location_schema_builder(ObjSchema& location_schema);
static void	cgi_schema_builder(ObjSchema& cgi_schema);

void	server_schema_builder(ObjSchema& server_schema)
{   
	IntSchema*	port_schema = new IntSchema("listen", true, true);

	port_schema->setMin(1);
	port_schema->setMax(65535);
	server_schema.addField(port_schema);
	server_schema.addField(new StringSchema("name", true, true));
	server_schema.addField(new StringSchema("root", true, false));
	server_schema.addField(new StringSchema("index", false, false));

	IntSchema*	body_size_schema = new IntSchema("max body", false, false);

	body_size_schema->setMin(0);
	body_size_schema->setMax(MAX_BODY_SIZE);
	server_schema.addField(body_size_schema);

	StringSchema*	interface_schema = new StringSchema("interface", false, true);

	interface_schema->addValidator(interface_validator);
	server_schema.addField(interface_schema);

	StringSchema*	allowed_requests_schema = new StringSchema("allowed requests", false, true);

	allowed_requests_schema->addValidator(request_validator);
	server_schema.addField(allowed_requests_schema);

	ObjSchema*	error_schema = new ObjSchema("error page", false, true);

	error_page_schema_builder(*error_schema);
	server_schema.addField(error_schema);

	ObjSchema*	location_schema = new ObjSchema("locations", false, true);
	
	location_schema_builder(*location_schema);
	server_schema.addField(new ObjSchema("location", false, true));
}

static void	error_page_schema_builder(ObjSchema& error_schema)
{
	IntSchema*	err_code_schema = new IntSchema("error code", true, false);

	err_code_schema->setMin(100);
	err_code_schema->setMax(999);
	error_schema.addField(err_code_schema);
	error_schema.addField(new StringSchema("error page", true, false));
}

static void	location_schema_builder(ObjSchema& location_schema)
{
	location_schema.addField(new StringSchema("name", true, false));
	location_schema.addField(new StringSchema("root", false, false));
	location_schema.addField(new StringSchema("index", false, false));

	StringSchema*	allowed_requests_schema = new StringSchema("allowed requests", false, true);

	allowed_requests_schema->addValidator(request_validator);
	location_schema.addField(allowed_requests_schema);

	StringSchema*	redirections_schema = new StringSchema("redirection", false, true);

	redirections_schema->addValidator(redirection_validator);

	location_schema.addField(new BoolSchema("directory listing", false, false));
	
	ObjSchema*	cgi_schema = new ObjSchema("cgi", false, true);

	cgi_schema_builder(*cgi_schema);
}

static void	cgi_schema_builder(ObjSchema& cgi_schema)
{
	StringSchema*	extension_schema = new StringSchema("extension", true, true);

	extension_schema->addValidator(extension_validator);
	cgi_schema.addField(extension_schema);
	cgi_schema.addField(new StringSchema("execution", true, true));
}