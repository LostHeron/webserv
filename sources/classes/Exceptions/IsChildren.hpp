/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IsChildren.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 12:47:02 by jweber            #+#    #+#             */
/*   Updated: 2026/05/30 12:59:20 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISCHILDREN_HPP
# define ISCHILDREN_HPP

// this class does not inherits std::exception on purpose !
class IsChildren
{
	public:
		const char *what() const throw();
};

#endif // !ISCHILDREN_HPP
