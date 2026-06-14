/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:55:00 by cviel             #+#    #+#             */
/*   Updated: 2026/06/11 17:00:31 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONOBJ_HPP
# define JSONOBJ_HPP

# include <stdint.h>
# include <string>
# include <vector>
# include <map>
# include <ostream>
# include "JsonLexer.hpp"

class JsonObj
{	
	public:

		typedef std::vector<JsonObj> Array;
		typedef std::map<std::string, JsonObj> SubObj;

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
		
		template <typename T>
		T const&	getVal(void) const;

		JsonObj::e_jsonType	getType(void) const;

	private:

		e_jsonType	_type;
		int64_t		_typeInt;
		bool		_typeBool;
		std::string	_typeString;
		Array		_typeArray;
		SubObj		_typeSubObj;

		JsonObj(void);
};

#endif // JSONOBJ_HPP
