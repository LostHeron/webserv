/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 10:09:22 by jweber            #+#    #+#             */
/*   Updated: 2026/07/02 11:58:40 by jweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <cstdio>
#include <cstring>
#include "status.hpp"

static void	change_run_status(int sig);
static int setup_sigint();
static int setup_sigpipe();

int	setup_signals()
{
	if (setup_sigpipe() != SUCCESS)
		return (FAILURE);
	if (setup_sigint() != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}


static int setup_sigpipe()
{
	struct sigaction	s;
	int					ret;

	std::memset(&s, 0, sizeof(s));
	s.sa_handler = SIG_IGN;
	ret = sigaction(SIGPIPE, &s, NULL);
	if (ret < 0)
	{
		return (FAILURE);
	}
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
	run = STOP;
}

