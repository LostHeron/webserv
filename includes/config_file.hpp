/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:25:38 by cviel             #+#    #+#             */
/*   Updated: 2026/07/03 15:49:59 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

# define MAX_BODY_SIZE 9223372036854775807
# define HOST_KEY "host"
# define HOST_PORT_KEY "listen"
# define HOST_NAME_KEY "name"
# define HOST_ROOT_KEY "root"
# define HOST_INDEX_KEY "index"
# define HOST_BODY_SIZE_KEY "max body"
# define HOST_INTERFACE_KEY "interface"
# define HOST_DIR_LIST_KEY  "directory listing"
# define HOST_ALLOWED_REQUEST_KEY "request"
# define HOST_ERROR_KEY "error"
# define HOST_LOCATION_KEY "location"
# define HOST_CGI_KEY "cgi"
# define HOST_CGI_EXT_KEY "extension"
# define HOST_UPLOAD_KEY "upload"
# define ERROR_CODE_KEY "code"
# define ERROR_PAGE_KEY "link"
# define LOC_NAME_KEY "name"
# define LOC_ALIAS_KEY "alias"
# define LOC_REDIRECTION_KEY "redirection"
# define LOC_INDEX_KEY "index"
# define LOC_BODY_SIZE_KEY "max body"
# define LOC_DIR_LIST_KEY "directory listing"
# define LOC_ALLOWED_REQUEST_KEY "request"
# define LOC_CGI_KEY "cgi"
# define LOC_CGI_EXT_KEY "extension"
# define LOC_UPLOAD_KEY "upload"

# include <stdint.h>
# include "ObjSchema.hpp"

void	host_schema_builder(ObjSchema& server_schema);
void	path_validator(std::string const& path);
void	index_validator(std::string const& index);
void	interface_validator(std::string const& interface);
void	request_validator(std::string const& request);
void	printable_validator(std::string const& str);

#endif