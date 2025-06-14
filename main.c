/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/14 15:39:39 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	initialize_hd_fd(t_step *step)
{
	t_step	*st;

	if (!step)
		return ;
	st = step;
	while (st)
	{
		st->hd_fd = -2;
		st = st->next;
	}
}

int	shell_execution(t_step **step, char ***env)
{
	int	status;

	status = 0;
	initialize_hd_fd(*step);
	handle_heredoc(*step, *env);
	if ((*step)->pipe)
		status = create_processes(*step, *env);
	else if (!((*step)->pipe))
		status = execute_single_cmd(*step, env);
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
			break ; // readline_err(line, status, env);
					// need to consider a bit more
		if (line[0])
		{
			add_history(line);
			status = parser(line, &step, env);
			exit_status(status, true);
			if (step)
				status = shell_execution(&step, &env);
			free(line);
			line = NULL;
			exit_status(status, true);
			fprintf(stderr, "I am exiting now\n");
			printf("%d %s\n", status, line);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	char	*line;
	t_step	*step;

	(void)argc;
	(void)argv;
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

/* void	del_all(t_del *del, int i)
{
	static int	status;
	static int	status;
	static int	status;

	if (del)
	{
		ft_free_strarr(*(del->env));
		st_lstclear(del->head);
	}
	if (i)
		exit(EXIT_FAILURE);
} */
