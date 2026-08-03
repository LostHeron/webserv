/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:36:00 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 13:41:39 by jweber           ###   ########.fr       */
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
# define CONTINUE_STATUS 1

# define TTL_CONNECTION_LAST_RECEIVED 60
# define TTL_CONNECTION_FULL 6000

extern int run;

#endif
