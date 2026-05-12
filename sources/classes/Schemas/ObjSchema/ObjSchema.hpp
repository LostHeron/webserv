/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 18:35:34 by cviel            ###   ########.fr       */
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

		ObjSchema(std::string const& name, bool is_required, bool allow_multiple);
		virtual ~ObjSchema();	
		
		void	addField(ASchema* schema);
		
	private:
		
		std::vector<ASchema*>	_fields;
		
		ObjSchema(void);
		ObjSchema(ObjSchema const& other);
		
		ObjSchema&	operator=(ObjSchema const& other);

		virtual void	checkValue(JsonObj const& object) const;
};

#endif // OBJSCHEMA_HPP