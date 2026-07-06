/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/07/01 18:35:35 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTSCHEMA_HPP
# define INTSCHEMA_HPP

# include <stdint.h>
# include <string>
# include "JsonObj.hpp"
# include "ASchema.hpp"

class IntSchema :
	public ASchema
{
	public:

		IntSchema(std::string const& name, bool is_required, bool allow_multiple);
		virtual ~IntSchema();
		
		void	setMin(int64_t min);
		void	setMax(int64_t max);
		
	private:
		
		int64_t	_min;
		int64_t	_max;
		
		IntSchema(void);
		IntSchema(IntSchema const& other);
		
		IntSchema&	operator=(IntSchema const& other);

		virtual void	checkValue(JsonObj const& object) const;
};

#endif // INTSCHEMA_HPP