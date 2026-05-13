/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:25:38 by cviel             #+#    #+#             */
/*   Updated: 2026/05/13 17:18:17 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

# define MAX_BODY_SIZE 2147483647
# define IP_SIZE 4

# include "ObjSchema.hpp"

void	host_setup(char const* filename);
void	server_schema_builder(ObjSchema& server_schema);
void	interface_validator(std::string const& interface);
void	request_validator(std::string const& request);
void	redirection_validator(std::string const& redir);
void	extension_validator(std::string const& extension);

#endif