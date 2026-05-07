/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASchema.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 20:28:04 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 19:41:29 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASCHEMA_HPP
# define ASCHEMA_HPP

# include <string>
# include "JsonObj.hpp"

class ASchema
{
	public:

		ASchema(std::string const& name, JsonObj::e_jsonType type, bool is_required, bool is_unique);
		ASchema(ASchema const& other);
		virtual ~ASchema();

		ASchema&	operator=(ASchema const& other);

		bool	validate(std::map<std::string, JsonObj> const& obj_map) const;

	private:

		std::string			_name;
		JsonObj::e_jsonType	_type;
		bool				_isRequired;
		bool				_allowMultiple;

		ASchema(void);

		bool			checkType(JsonObj const& object) const;
		virtual bool	checkValue(JsonObj const& object) const = 0;
};

#endif // ASCHEMA_HPP