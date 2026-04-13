/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:50:25 by cviel             #+#    #+#             */
/*   Updated: 2026/04/13 19:23:32 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "JsonLexer.hpp"
#include "JsonObj.hpp"
#include <iostream>

void    host_setup(char const* filename)
{
	std::ifstream   config_file(filename);

	if (config_file.fail())
		throw std::runtime_error("File stream failed to properly setup");
	
	std::stringstream	input_stream;

	input_stream << config_file.rdbuf();
	
	JsonLexer						lexer(input_stream.str());
	std::map<std::string, JsonObj>	objects;


	while (!lexer.empty())
	{
		if (lexer.peekType() != JsonLexer::STRING)
			throw std::invalid_argument("Invalid key");
		
		std::string	key;

		key = lexer.popToken();
		if (lexer.peekType() != JsonLexer::COLON)
			throw std::invalid_argument("Missing ':' separator between key and value");
		lexer.popToken();

		JsonObj	val(lexer);

		if (!objects.insert(std::pair<std::string, JsonObj>(key, val)).second)
			throw std::runtime_error("Insertion failed");
	}
}