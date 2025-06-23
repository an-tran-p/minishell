/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 18:00:11 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint = SIGINT_NONE;

void	sigint_parent_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	if (!g_sigint)
		g_sigint = SIGINT_READLINE;
	if (g_sigint == SIGINT_READLINE)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		exit_status(130, true);
	}
	if (g_sigint > 0)
		g_sigint = SIGINT_NONE;
}

void	sigint_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint = SIGINT_HEREDOC_RECEIVED;
	}
}

int	sig_hook(void)
{
	if (g_sigint == SIGINT_HEREDOC_RECEIVED)
		rl_done = 1;
	return (0);
}
