/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:25:38 by cviel             #+#    #+#             */
/*   Updated: 2026/05/18 19:18:28 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

# define HOST_KEY "host"
# define HOST_PORT_KEY "listen"
# define HOST_NAME_KEY "name"
# define HOST_ROOT_KEY "root"
# define HOST_INDEX_KEY "index"
# define HOST_BODY_SIZE_KEY "max body"
# define MAX_BODY_SIZE 2147483647
# define HOST_INTERFACE_KEY "interface"
# define HOST_DIR_LIST_KEY  "directory listing"
# define HOST_ALLOWED_REQUEST_KEY "request"
# define HOST_ERROR_KEY "error"
# define HOST_LOCATION_KEY "location"
# define HOST_CGI_KEY "cgi"
# define ERROR_CODE_KEY "code"
# define ERROR_PAGE_KEY "link"
# define LOC_NAME_KEY "name"
# define LOC_ROOT_KEY "root"
# define LOC_INDEX_KEY "index"
# define LOC_DIR_LIST_KEY "directory listing"
# define LOC_ALLOWED_REQUEST_KEY "reqeusts"
# define LOC_REDIRECTION_KEY "redirection"
# define LOC_CGI_KEY "cgi"
# define CGI_EXTENSION_KEY "extension"
# define CGI_EXEC_KEY "execution"

# include "VirtualHost.hpp"
# include "ObjSchema.hpp"

void	host_setup(char const* filename, std::map<u_int16_t, VirtualHost> host_map);
void	host_schema_builder(ObjSchema& server_schema);
void	interface_validator(std::string const& interface);
void	request_validator(std::string const& request);
void	redirection_validator(std::string const& redir);
void	extension_validator(std::string const& extension);
void	non_empty_validator(std::string const& str);

#endif