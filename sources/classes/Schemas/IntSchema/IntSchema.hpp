/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/06/09 16:36:27 by cviel            ###   ########.fr       */
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
		
		void	setMin(int min);
		void	setMax(int max);
		
	private:
		
		int	_min;
		int	_max;
		
		IntSchema(void);
		IntSchema(IntSchema const& other);
		
		IntSchema&	operator=(IntSchema const& other);

		virtual void	checkValue(JsonObj const& object) const;
};

#endif // INTSCHEMA_HPP