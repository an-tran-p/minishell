/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/22 23:26:01 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sigint;

int	exit_status(int new_s, bool add)
{
	static int	status;

	if (add)
		status = new_s;
	return (status);
}

int	shell_execution(t_step **step, char ***env)
{
	int	status;

	status = 0;
	initialize_hd_fd(*step);
	handle_heredoc(*step, *env);
	status = exit_status(0, false);
	if (status == 130 && g_sigint == SIGINT_HEREDOC_RECEIVED)
	{
		close_hd(*step);
		st_lstclear(step);
		g_sigint = SIGINT_NONE;
		return (130);
	}
	if ((*step)->pipe)
		status = create_processes(*step, *env);
	else if (!((*step)->pipe))
		status = execute_single_cmd(*step, env);
	close_hd(*step);
	st_lstclear(step);
	return (status);
}

void	minishell(int status, char *line, t_step *step, char **envp)
{
	char	**env;

	env = copy_env(envp);
	if (!env)
		exit(EXIT_FAILURE);
	while (1)
	{
		line = readline("minishell% ");
		if (!line)
			break ;
		if (line[0])
		{
			add_history(line);
			status = parser(line, &step, env);
			if (step)
				status = shell_execution(&step, &env);
			free(line);
			line = NULL;
			exit_status(status, true);
		}
	}
	rl_clear_history();
	ft_free_strarr(env);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	char	*line;
	t_step	*step;

	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_parent_handler);
	signal(SIGQUIT, SIG_IGN);
	step = NULL;
	line = NULL;
	status = 0;
	minishell(status, line, step, envp);
}
