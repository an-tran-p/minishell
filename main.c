/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/18 21:19:50 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	heredoc_interrupted;

int	exit_status(int new_s, bool add)
{
	static int	status;

	if (add)
		status = new_s;
	return (status);
}

// void	readline_err(char *line, int status, char **env)
// {
// 	// memory failure? or end program
// 	(void)env;
// 	(void)status;
// 	fprintf(stderr, "I am in rl_eer");
// 	if (!line)
// 	{
// 		/* ft_free_strarr(env); */
// 		fprintf(stderr, "I am in rl_eer ENONEM");
// 		/* if (errno == ENOMEM)
// 		{
// 			exit(EXIT_FAILURE);
// 		}
// 		else */
// 		return ; //(status);
// 	}
// }

int	shell_execution(t_step **step, char ***env)
{
	int	status;

	status = 0;
	initialize_hd_fd(*step);
	if (handle_heredoc(*step, *env) == -3)
	{
		st_lstclear(step);
		heredoc_interrupted = 0;
		rl_catch_signals = 1;
		status = 130;
		return (status);
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
	signal(SIGINT, handling_sigint);
	signal(SIGQUIT, SIG_IGN);
	step = NULL;
	line = NULL;
	status = 0;
	minishell(status, line, step, envp);
}
/*
int	main(int argc, char **argv, char **envp)
{
	int		status;
	char	*line;
	char	**env;
	t_step	*step;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	if (!env)
		return (EXIT_FAILURE);
	step = NULL;
	status = 0;
	while (1)
	{
		line = readline("minishell% ");
		if (!line)
			readline_err(line, status, env);
		if (line[0])
		{
			add_history(line);
			status = parser(line, &step);
free(line);
line = NULL;
printf("%d %s\n", status, line);
			if (!step)
				status = shell_execution(&step, env);
		}
	}
}
*/
