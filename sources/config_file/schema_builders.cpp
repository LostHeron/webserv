/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema_builders.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:22:01 by cviel             #+#    #+#             */
/*   Updated: 2026/05/20 17:42:04 by cviel            ###   ########.fr       */
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

void	host_schema_builder(ObjSchema& host_schema)
{   
	IntSchema*	port_schema = new IntSchema(HOST_PORT_KEY, true, false);

	port_schema->setMin(1);
	port_schema->setMax(65535);
	host_schema.addField(port_schema);

	StringSchema*	name_schema = new StringSchema(HOST_NAME_KEY, true, true);
	
	name_schema->addValidator(non_empty_validator);
	host_schema.addField(name_schema);

	StringSchema*	root_schema = new StringSchema(HOST_ROOT_KEY, true, false);
	
	name_schema->addValidator(non_empty_validator);
	host_schema.addField(root_schema);

	StringSchema*	index_schema = new StringSchema(HOST_INDEX_KEY, false, false);
	
	name_schema->addValidator(non_empty_validator);
	host_schema.addField(index_schema);

	IntSchema*	body_size_schema = new IntSchema(HOST_BODY_SIZE_KEY, false, false);

	body_size_schema->setMin(0);
	body_size_schema->setMax(MAX_BODY_SIZE);
	host_schema.addField(body_size_schema);

	StringSchema*	interface_schema = new StringSchema(HOST_INTERFACE_KEY, false, true);

	interface_schema->addValidator(interface_validator);
	host_schema.addField(interface_schema);

	StringSchema*	allowed_requests_schema = new StringSchema(HOST_ALLOWED_REQUEST_KEY, false, true);

	allowed_requests_schema->addValidator(request_validator);
	host_schema.addField(allowed_requests_schema);

	host_schema.addField(new BoolSchema(HOST_DIR_LIST_KEY, false, false));

	ObjSchema*	error_schema = new ObjSchema(HOST_ERROR_KEY, false, true);

	error_page_schema_builder(*error_schema);
	host_schema.addField(error_schema);

	ObjSchema*	location_schema = new ObjSchema(HOST_LOCATION_KEY, false, true);
	
	location_schema_builder(*location_schema);
	host_schema.addField(location_schema);

	ObjSchema*	cgi_schema = new ObjSchema(HOST_CGI_KEY, false, true);

	cgi_schema_builder(*cgi_schema);
	host_schema.addField(cgi_schema);
}

static void	error_page_schema_builder(ObjSchema& error_schema)
{
	IntSchema*	err_code_schema = new IntSchema(ERROR_CODE_KEY, true, false);

	err_code_schema->setMin(100);
	err_code_schema->setMax(999);
	error_schema.addField(err_code_schema);

	StringSchema*	err_page_schema = new StringSchema(ERROR_PAGE_KEY, true, false);
	
	err_page_schema->addValidator(non_empty_validator);
	error_schema.addField(err_page_schema);
}

static void	location_schema_builder(ObjSchema& location_schema)
{
	StringSchema*	name_schema = new StringSchema(LOC_NAME_KEY, true, false);

	name_schema->addValidator(non_empty_validator);
	location_schema.addField(name_schema);

	StringSchema*	root_schema = new StringSchema(LOC_ROOT_KEY, true, false);

	root_schema->addValidator(non_empty_validator);
	location_schema.addField(root_schema);

	StringSchema*	index_schema = new StringSchema(LOC_INDEX_KEY, false, false);

	index_schema->addValidator(non_empty_validator);
	location_schema.addField(index_schema);

	StringSchema*	allowed_requests_schema = new StringSchema(LOC_ALLOWED_REQUEST_KEY, false, true);

	allowed_requests_schema->addValidator(request_validator);
	location_schema.addField(allowed_requests_schema);
	location_schema.addField(new BoolSchema(LOC_DIR_LIST_KEY, false, false));

	StringSchema*	redirections_schema = new StringSchema(LOC_REDIRECTION_KEY, false, true);

	redirections_schema->addValidator(redirection_validator);
	
	ObjSchema*	cgi_schema = new ObjSchema(LOC_CGI_KEY, false, true);

	cgi_schema_builder(*cgi_schema);
}

static void	cgi_schema_builder(ObjSchema& cgi_schema)
{
	StringSchema*	extension_schema = new StringSchema(CGI_EXTENSION_KEY, true, true);

	extension_schema->addValidator(non_empty_validator);
	cgi_schema.addField(extension_schema);

	StringSchema*	exec_schema = new StringSchema(CGI_EXEC_KEY, true, true);

	exec_schema->addValidator(non_empty_validator);
	cgi_schema.addField(exec_schema);
}
