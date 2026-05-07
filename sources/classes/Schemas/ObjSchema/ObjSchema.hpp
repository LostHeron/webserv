/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 18:32:57 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJSCHEMA_HPP
# define OBJSCHEMA_HPP

# include <string>
# include <vector>
# include "JsonObj.hpp"
# include "ASchema.hpp"

class ObjSchema :
	public ASchema
{
	public:

		ObjSchema(std::string const& name, bool is_required, bool is_unique);
		ObjSchema(ObjSchema const& other);
		virtual ~ObjSchema();

		ObjSchema&	operator=(ObjSchema const& other);

		void	addField(ASchema* schema);

	private:

		std::vector<ASchema*>	_fields;

		ObjSchema(void);

		virtual bool	checkValue(JsonObj const& object);
}


#endif // OBJSCHEMA_HPP