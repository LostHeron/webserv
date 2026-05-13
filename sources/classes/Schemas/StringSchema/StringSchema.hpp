/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringSchema.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/13 14:55:53 by cviel            ###   ########.fr       */
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

		StringSchema(std::string const& name, bool is_required, bool allow_multiple);
		virtual ~StringSchema();

		void	addValidator(void (*validator)(std::string const&));
		
	private:

		void	(*_validator)(std::string const&);
		
		StringSchema(void);
		StringSchema(StringSchema const& other);
		
		StringSchema&	operator=(StringSchema const& other);

		virtual void	checkValue(JsonObj const& object) const;
};

#endif // STRINGSCHEMA_HPP