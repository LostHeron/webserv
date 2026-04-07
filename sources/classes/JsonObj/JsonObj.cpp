/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:18:42 by cviel             #+#    #+#             */
/*   Updated: 2026/04/07 18:27:23 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include "JsonObj.hpp"

JsonObj::JsonObj(JsonLexer& jsonLexer) :
	_lexer(jsonLexer),
	_type(NONE)
{}

JsonObj::JsonObj(JsonObj const& other) :
	_lexer(other._lexer),
	_type(other._type),
	_key(other._key)
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
		this->_lexer = other._lexer;
		this->_type = other._type;
		this->_key = other._key;
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

std::string const&	JsonObj::getKey(void) const
{
	return (this->_key);	
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
