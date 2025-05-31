/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/30 23:03:25 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test_cases.c"

int	create_processes(t_pipe_step *input, int step, char **env)
{
	int		fds[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Failed creating process\n");
		return (1);
	}
	if (pid1 == 0)
		execute_child1_process(fds, argv[1], argv[2], envp);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Failed creating process\n");
		return (1);
	}
	if (pid2 == 0)
		execute_child2_process(fds, argv[4], argv[3], envp);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

int	count_steps(t_pipe_step *input)
{
	int			step;
	t_pipe_step	*dup;

	step = 0;
	dup = input;
	while (dup)
	{
		if (dup->pipe == 1)
			step++;
		dup = dup->next;
	}
	return (step);
}

int	main(int argc, char **argv, char **envp)
{
	char		**env;
	char		*line;
	t_pipe_step	*input;
	int			step;
	int			status;

	(void)argc;
	env = copy_env(envp);
	if (!env)
		return (-1);
	/* if (is_builtins(argv[1]) == 0)
	{
		execute_builtin(&argv[1], &env);
		export_print(env);
	} */
	input = test_1();
	step = count_steps(input);
	status = 0;
	if (step > 0)
		status = create_processes(input, step, env);
	ft_free_strarr(env);
	return (status);
}
