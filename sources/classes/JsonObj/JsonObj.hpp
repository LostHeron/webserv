/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObj.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:55:00 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 17:55:31 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSONOBJ_HPP
# define JSONOBJ_HPP

# include <string>
# include <vector>
# include <map>
# include <ostream>
# include "JsonLexer.hpp"

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
	
		e_jsonType								getType(void) const;
		int										getInt(void) const;
		bool									getBool(void) const;
		std::string const&						getString(void) const;
		std::vector<JsonObj> const&				getArray(void) const;
		std::map<std::string, JsonObj> const&	getSubObj(void) const;

		void	print(std::ostream& out) const;
	
	private:

		e_jsonType						_type;
		int								_typeInt;
		bool							_typeBool;
		std::string						_typeString;
		std::vector<JsonObj>			_typeArray;
		std::map<std::string, JsonObj>	_typeSubObj;

		JsonObj(void);
};

#endif // JSONOBJ_HPP