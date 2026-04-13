/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:18:42 by cviel             #+#    #+#             */
/*   Updated: 2026/04/13 17:08:04 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <map>
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
			break ;
		}
		case JsonLexer::OBJ_OPEN:
		{
			while (jsonLexer.peekType() != JsonLexer::OBJ_CLOSE)
			{
				if (jsonLexer.peekType() != JsonLexer::COMMA && jsonLexer.peekType() != JsonLexer::OBJ_OPEN)
					throw std::invalid_argument("Missing comma between values in array");
				jsonLexer.popToken();
				if (jsonLexer.peekType() != JsonLexer::STRING)
					throw std::invalid_argument("Invalid key");
				
				std::string	key;

				key = jsonLexer.popToken();
				if (jsonLexer.peekType() != JsonLexer::COLON)
					throw std::invalid_argument("Missing ':' separator between key and value");
				jsonLexer.popToken();

				JsonObj	val(jsonLexer);

				if (!this->_typeSubObj.insert({key, val}).second)
					throw std::runtime_error("Insertion failed");
			}
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

template <typename T>
T const&	JsonObj::getValue(void) const
{
	switch (this->_type)
	{
		case INT:
		{
			return (this->_typeInt);
		}
		case BOOL:
		{
			return (this->_typeBool);
		}
		case STRING:
		{
			return (this->_typeString);
		}
		case ARRAY:
		{
			return (this->_typeArray);
		}
		case SUBOBJ:
		{
			return (this->_typeSubObj);
		}
		default:
		{
			throw std::runtime_error("JsonObj has unknown or invalid type : Cannot get value");
		}
	}
}
