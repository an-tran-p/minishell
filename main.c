/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/11 23:39:21 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	del_all(t_del *del, int i)
{
	if (del)
	{
		ft_free_strarr(*(del->env));
		st_lstclear(del->head);
	}
	if (i)
		exit(EXIT_FAILURE);
} */
void	readline_err(char *line, int status, char **env)
{
	// memory failure? or end program
	if (!line)
	{
		ft_free_strarr(env);
		if (errno == ENOMEM)
		{
			exit(EXIT_FAILURE);
		}
		else
			exit(status);
	}
}

int	shell_execution(t_step *step, char **env)
{
	int		status;
	t_del	del;

	status = 0;
	del.env = &env;
	del.head = &step;
	handle_heredoc(step);
	if (step->pipe)
		status = create_processes(step, env);
	else
		status = execute_single_cmd(step, &env);
	ft_free_strarr(env);
	st_lstclear(&step);
	return (status);
}

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
			if (!status)
				status = shell_execution(step, env);
		}
	}
}
