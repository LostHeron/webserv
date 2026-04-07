/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:55:00 by cviel             #+#    #+#             */
/*   Updated: 2026/04/07 19:57:07 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JSONOBJ_HPP__
# define __JSONOBJ_HPP__

#include <string>
#include <array>
#include <vector>
#include <map>
#include "JsonLexer.hpp"

class JsonObj
{	
	public:

		enum e_jsonType
		{
			NONE,
			INT,
			BOOL,
			STRING,
			ARRAY,
			SUBOBJ
		};
	
		JsonObj(JsonLexer& jsonLexer);
		JsonObj(JsonObj const& other);
		~JsonObj();
	
		JsonObj&	operator=(JsonObj const& other);
	
		e_jsonType			getType(void) const;
		std::string const&	getKey(void) const;
		template <typename T>
		T const&			getValue(void) const;
	
	private:

		JsonLexer&						_lexer;
		e_jsonType						_type;
		std::string						_key;
		int								_typeInt;
		bool							_typeBool;
		std::string						_typeString;
		std::vector<JsonObj>			_typeArray;
		std::map<std::string, JsonObj>	_typeSubObj;

		JsonObj(void);
};

#endif