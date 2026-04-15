/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 10:09:22 by jweber            #+#    #+#             */
/*   Updated: 2026/04/15 10:25:24 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstring>
#include "status.hpp"

static void	change_run_status(int sig);
static int setup_sigint();

int	setup_signals()
{
	if (setup_sigint() != SUCCESS)
		return (FAILURE);
	// TODO ALSO CATCH SIGPIPE to avoid the server from
	// crashing when peer end closes its end
	// or use sendto with flag asking it to not send
	// signals in case of other end closed.
	return (SUCCESS);
}


static int setup_sigint()
{
	struct sigaction	s;
	int					ret;

	std::memset(&s, 0, sizeof(s));
	s.sa_handler = &change_run_status;
	s.sa_flags = SA_RESTART;
	ret = sigaction(SIGINT, &s, NULL);
	if (ret < 0)
	{
		return (FAILURE);
	}
	return (SUCCESS);
}

static void	change_run_status(int sig)
{
	(void) sig;
	run = 0;
}

