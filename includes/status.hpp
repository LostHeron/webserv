/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:36:00 by jweber            #+#    #+#             */
/*   Updated: 2026/06/02 11:57:41 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS
# define STATUS

enum {
	SUCCESS=0,
	FAILURE,
	QUIT,
	FINISH,
	TERMINATE,
};

# define STOP 0
# define CONTINUE 1

# define TTL_CONNECTION 6

extern int run;

#endif
