/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_file.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:25:38 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 19:27:33 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

#include "ObjSchema.hpp"

void    host_setup(char const* filename);
void    server_schema_builder(ObjSchema& server_schema);

#endif