/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 17:25:08 by cviel             #+#    #+#             */
/*   Updated: 2026/04/13 16:32:39 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <string>
#include <queue>
#include "JsonLexer.hpp"

JsonLexer::JsonLexer(std::string const& input)
{
	for (std::string::size_type i = 0; i < input.size(); ++i)
	{
		while (i < input.size() && std::isspace(static_cast<unsigned char>(input[i])))
			++i;
		if (i >= input.size())
			break ;

		struct s_token	new_token;

		switch (input[i])
		{
			case '{':
			{
				new_token.type = OBJ_OPEN;
				break ;
			}
			case '}':
			{
				new_token.type = OBJ_CLOSE;
				break ;
			}
			case '[':
			{
				new_token.type = ARRAY_OPEN;
				break ;
			}
			case ']':
			{
				new_token.type = ARRAY_CLOSE;
				break ;
			}
			case ':':
			{
				new_token.type = COLON;
				break ;
			}
			case ',':
			{
				new_token.type = COMMA;
				break ;
			}
			case '"':
			{
				new_token.type = STRING;
				while (i < input.size() && input[i] != '"')
				{
					if (input[i] != '\\')
						++i;
					if (i < input.size())
						new_token.value.push_back(input[i]);
					++i;
				}
				if (i >= input.size())
					throw std::invalid_argument("Invalid argument : missing closing quotes at \"" + input.substr(input.rfind('"', i), 10) + "...\"");
				++i;
				break ;
			}
			default:
			{
				if (input.compare(i, 4, "true") == 0)
				{
					new_token.type = BOOL_TRUE;
					i += 4;
					break ;
				}
				if (input.compare(i, 5, "false") == 0)
				{
					new_token.type = BOOL_FALSE;
					i += 5;
					break ;
				}
				new_token.type = INT;
				while (i < input.size() && std::isdigit(static_cast<unsigned char>(input[i])))
				{
					new_token.value.push_back(input[i]);
					++i;
				}
				if (new_token.value.empty())
					throw std::invalid_argument("Invalid argument : unrecognized sequence of characters at \"" + input.substr(i, 10) + "...\"");
			}
		}
		this->_tokenQueue.push(new_token);
	}
}

JsonLexer::JsonLexer(JsonLexer const& other) :
	_tokenQueue(other._tokenQueue)
{}

JsonLexer::~JsonLexer()
{}

JsonLexer&	JsonLexer::operator=(JsonLexer const& other)
{
	if (this != &other)
	{
		this->_tokenQueue = other._tokenQueue;
	}
	return (*this);
}

bool	JsonLexer::empty(void) const
{
	return (this->_tokenQueue.empty());	
}

JsonLexer::e_tokenType	JsonLexer::peekType(void) const
{
	if (this->_tokenQueue.empty())
		throw std::range_error("Lexer is empty : trying to access non-existant element");
	return (this->_tokenQueue.front().type);
}

std::string	JsonLexer::popToken(void)
{
	if (this->_tokenQueue.empty())
		throw std::range_error("Lexer is empty : trying to access non-existant element");
	
	std::string	token_value(this->_tokenQueue.front().value);

	this->_tokenQueue.pop();
	return (token_value);
}