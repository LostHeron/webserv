/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:18:42 by cviel             #+#    #+#             */
/*   Updated: 2026/06/11 17:09:50 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <ostream>
#include "JsonLexer.hpp"
#include "JsonObj.hpp"

JsonObj::JsonObj(JsonLexer& jsonLexer)
{
	switch (jsonLexer.peekType())
	{
		case JsonLexer::INT:
		{
			std::stringstream	convert(jsonLexer.popToken());
			
			this->_type = JsonObj::INT;
			convert >> this->_typeInt;
			break ;
		}
		case JsonLexer::BOOL_TRUE:
		{
			this->_type = JsonObj::BOOL;
			this->_typeBool = true;
			jsonLexer.popToken();
			break ;
		}
		case JsonLexer::BOOL_FALSE:
		{
			this->_type = JsonObj::BOOL;
			this->_typeBool = false;
			jsonLexer.popToken();
			break ;
		}
		case JsonLexer::STRING:
		{
			this->_type = JsonObj::STRING;
			this->_typeString = jsonLexer.popToken();
			break ;
		}
		case JsonLexer::ARRAY_OPEN:
		{
			while (jsonLexer.peekType() != JsonLexer::ARRAY_CLOSE)
			{
				if (jsonLexer.peekType() != JsonLexer::COMMA && jsonLexer.peekType() != JsonLexer::ARRAY_OPEN)
					throw std::invalid_argument("Missing comma between values in array");
				jsonLexer.popToken();

				JsonObj	val(jsonLexer);

				this->_typeArray.push_back(val);
			}
			if (this->_typeArray.empty() == true)
				throw std::invalid_argument("Array is empty");
			this->_type = JsonObj::ARRAY;
			jsonLexer.popToken();
			break ;
		}
		case JsonLexer::OBJ_OPEN:
		{
			while (jsonLexer.peekType() != JsonLexer::OBJ_CLOSE)
			{
				if (jsonLexer.peekType() != JsonLexer::COMMA && jsonLexer.peekType() != JsonLexer::OBJ_OPEN)
					throw std::invalid_argument("Missing comma between values in object");
				jsonLexer.popToken();
				if (jsonLexer.peekType() != JsonLexer::STRING)
					throw std::invalid_argument("Invalid key");
				
				std::string	key;

				key = jsonLexer.popToken();
				if (jsonLexer.peekType() != JsonLexer::COLON)
					throw std::invalid_argument("Missing ':' separator between key and value");
				jsonLexer.popToken();

				JsonObj	val(jsonLexer);

				if (!this->_typeSubObj.insert(std::pair<std::string, JsonObj>(key, val)).second)
					throw std::runtime_error("An element with the same key already exists");
			}
			this->_type = JsonObj::SUBOBJ;
			jsonLexer.popToken();
			break ;
		}
		default:
		{
			throw std::invalid_argument("Unexpected token");
		}
	}
}

JsonObj::JsonObj(JsonObj const& other) :
	_type(other._type)
{
	switch (this->_type)
	{
		case NONE:
		{
			break;
		}
		case INT:
		{
			this->_typeInt = other._typeInt;
			break ;
		}
		case BOOL:
		{
			this->_typeBool = other._typeBool;
			break ;
		}
		case STRING:
		{
			this->_typeString = other._typeString;
			break ;
		}
		case ARRAY:
		{
			this->_typeArray = other._typeArray;
			break ;
		}
		case SUBOBJ:
		{
			this->_typeSubObj = other._typeSubObj;
			break ;
		}
		default:
		{
			throw std::runtime_error("JsonObj has unknown type : Aborting copy construction");
		}
	}
}

JsonObj::~JsonObj()
{}

JsonObj&    JsonObj::operator=(JsonObj const& other)
{
	if (this != & other)
	{
		this->_type = other._type;
		switch (this->_type)
		{
			case JsonObj::NONE:
			{
				break;
			}
			case JsonObj::INT:
			{
				this->_typeInt = other._typeInt;
				break ;
			}
			case JsonObj::BOOL:
			{
				this->_typeBool = other._typeBool;
				break ;
			}
			case JsonObj::STRING:
			{
				this->_typeString = other._typeString;
				break ;
			}
			case JsonObj::ARRAY:
			{
				this->_typeArray = other._typeArray;
				break ;
			}
			case JsonObj::SUBOBJ:
			{
				this->_typeSubObj = other._typeSubObj;
				break ;
			}
			default:
			{
				throw std::runtime_error("JsonObj has unknown type : Aborting copy assignment");
			}
		}
    }
    return (*this);
}

JsonObj::e_jsonType	JsonObj::getType(void) const
{
	return (this->_type);
}

template <>
int64_t const&	JsonObj::getVal<int64_t>(void) const
{
	if (this->_type != JsonObj::INT)
		throw std::logic_error("Incorrect type : JsonObj is not an integer");
	return (this->_typeInt);
}

template <>
uint16_t const&	JsonObj::getVal<uint16_t>(void) const
{
	if (this->_type != JsonObj::INT)
		throw std::logic_error("Incorrect type : JsonObj is not an integer");
	return (*(reinterpret_cast<const uint16_t*>(&this->_typeInt)));
}

template <>
bool const&	JsonObj::getVal<bool>(void) const
{
	if (this->_type != JsonObj::BOOL)
		throw std::logic_error("Incorrect type : JsonObj is not a boolean");
	return (this->_typeBool);
}

template <>
std::string const&	JsonObj::getVal<std::string>(void) const
{
	if (this->_type != JsonObj::STRING)
		throw std::logic_error("Incorrect type : JsonObj is not a string");
	return (this->_typeString);
}

template <>
JsonObj::Array const&	JsonObj::getVal<JsonObj::Array>(void) const
{
	if (this->_type != JsonObj::ARRAY)
		throw std::logic_error("Incorrect type : JsonObj is not an array");
	return (this->_typeArray);
}

template <>
JsonObj::SubObj const&	JsonObj::getVal<JsonObj::SubObj>(void) const
{
	if (this->_type != JsonObj::SUBOBJ)
		throw std::logic_error("Incorrect type : JsonObj is not a sub object");
	return (this->_typeSubObj);
}
