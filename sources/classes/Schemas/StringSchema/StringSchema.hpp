/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringSchema.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/12 19:16:17 by cviel            ###   ########.fr       */
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

		void	addValidator(void (*validator)(void));
		
	private:

		void	(*_validator)(void);
		
		StringSchema(void);
		StringSchema(StringSchema const& other);
		
		StringSchema&	operator=(StringSchema const& other);

		virtual void	checkValue(JsonObj const& object) const;
};

#endif // STRINGSCHEMA_HPP