/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoolSchema.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 15:03:51 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOOLSCHEMA_HPP
# define BOOLSCHEMA_HPP

# include <string>
# include "JsonObj.hpp"
# include "ASchema.hpp"

class BoolSchema :
	public ASchema
{
	public:

		BoolSchema(std::string const& name, bool is_required, bool is_unique);
		virtual ~BoolSchema();	
		
	private:
		
		BoolSchema(void);
		BoolSchema(BoolSchema const& other);
		
		BoolSchema&	operator=(BoolSchema const& other);
		
		virtual bool	checkValue(JsonObj const& object);
};

#endif // BOOLSCHEMA_HPP