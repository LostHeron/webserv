/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringSchema.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 18:26:54 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGSCHEMA_HPP
# define STRINGSCHEMA_HPP

# include <string>
# include "JsonObj.hpp"
# include "ASchema.hpp"

class StringSchema :
	public ASchema
{
	public:

		StringSchema(std::string const& name, bool is_required, bool is_unique);
		StringSchema(StringSchema const& other);
		virtual ~StringSchema();

		StringSchema&	operator=(StringSchema const& other);

	private:

		StringSchema(void);

		virtual bool	checkValue(JsonObj const& object);
}


#endif // STRINGSCHEMA_HPP