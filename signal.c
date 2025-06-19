/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/19 21:38:23 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	sigint = SIGINT_NONE;

void	sigint_parent_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	sigint = 1;
	fprintf(stderr, "signal is %d\n", sigint);
	rl_redisplay();
}

void	sigint_child_handler(int sig)
{
	(void)sig;
	exit(130);
}

void	handling_sigint_heredoc(int sig)
{
	(void)sig;
	sigint = 2;
	fprintf(stderr, "heredoc_interupted is %d\n", sigint);
	rl_replace_line("", 0);
	rl_on_new_line();
	write(STDOUT_FILENO, "\n", 1);
	exit(1);
}
