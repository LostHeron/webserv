/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IntSchema.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cviel <cviel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:30:18 by cviel             #+#    #+#             */
/*   Updated: 2026/05/07 17:49:16 by cviel            ###   ########.fr       */
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
		IntSchema(IntSchema const& other);
		virtual ~IntSchema();

		IntSchema&	operator=(IntSchema const& other);

		void	setMin(int min);
		void	setMax(int max);

	private:

		int	_min;
		int	_max;

		IntSchema(void);

		virtual bool	checkValue(JsonObj const& object);
}


#endif // INTSCHEMA_HPP