/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:18:42 by cviel             #+#    #+#             */
/*   Updated: 2026/05/20 17:22:38 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	JsonObj::getInt(void) const
{
	if (this->_type != INT)
		throw std::runtime_error("Using integer getter for a non integer");
	return (this->_typeInt);
}

bool	JsonObj::getBool(void) const
{
	if (this->_type != BOOL)
		throw std::runtime_error("Using boolean getter for a non boolean");
	return (this->_typeBool);
}

std::string	const&	JsonObj::getString(void) const
{
	if (this->_type != STRING)
		throw std::runtime_error("Using string getter for a non string");
	return (this->_typeString);
}

std::vector<JsonObj> const&	JsonObj::getArray(void) const
{
	if (this->_type != ARRAY)
		throw std::runtime_error("Using array getter for a non array");
	return (this->_typeArray);
}

std::map<std::string, JsonObj> const&	JsonObj::getSubObj(void) const
{
	if (this->_type != SUBOBJ)
		throw std::runtime_error("Using nested object getter for a non nested object");
	return (this->_typeSubObj);
}

void	JsonObj::print(std::ostream& out) const
{
	switch (this->_type)
	{
		case INT:
		{
			out << this->_typeInt;
			break ;
		}
		case BOOL:
		{
			out << this->_typeBool;
			break ;
		}
		case STRING:
		{
			out << this->_typeString;
			break ;
		}
		case ARRAY:
		{
			out << "[";
			for (std::vector<JsonObj>::const_iterator it = this->_typeArray.begin(); it != this->_typeArray.end(); ++it)
			{
				it->print(out);
				out << ",";
			}
			out << "]";
			break ;
		}
		case SUBOBJ:
		{
			out << "{";
			for (std::map<std::string, JsonObj>::const_iterator it = this->_typeSubObj.begin(); it != this->_typeSubObj.end(); ++it)
			{
				out << std::endl;
				out << it->first << " : ";
				it->second.print(out);
			}
			out << "}";
			break ;
		}
		default:
		{
			out << "Unknown JsonObj type";
		}
	}
}