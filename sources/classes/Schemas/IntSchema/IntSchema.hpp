/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/11 15:49:13 by cviel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTSCHEMA_HPP
# define INTSCHEMA_HPP

# include <string>
# include "JsonObj.hpp"
# include "ASchema.hpp"

class IntSchema :
	public ASchema
{
	public:

		IntSchema(std::string const& name, bool is_required, bool is_unique);
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