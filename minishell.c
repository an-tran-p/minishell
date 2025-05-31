/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/31 23:15:26 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test_cases.c"

void	execute(char **cmd, char **env)
{
	char	*path;

	path = find_path(cmd[0], env);
	if (!path)
	{
		ft_put_err("command not found: ", cmd[0]);
		ft_free_strarr(cmd);
		exit(127);
	}
	if (execve(path, cmd, env) == -1)
	{
		ft_free_strarr(cmd);
		free(path);
		perror("Execution fails\n");
		exit(127);
	}
	ft_free_strarr(cmd);
	free(path);
	exit(0);
}

void	execute_child_process(int *fds, int prev_fd, t_step *step, char **env)
{
	int	fd_in;
	int	fd_out;

	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (step->pipe)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[0]);
		close(fds[1]);
	}
	while (step->rd)
	{
		if (step->rd->type == RD_INFILE)
		{
			fd_in = open(step->rd->s, O_RDONLY);
			if (fd_in == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), step->rd->s);
				exit(1);
			}
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		if (step->rd->type == RD_OUTFILE || step->rd->type == RD_APPEND)
		{
			if (step->rd->type == RD_OUTFILE)
				fd_out = open(step->rd->s, O_RDWR | O_CREAT | O_TRUNC, 0777);
			else if (step->rd->type == RD_APPEND)
				fd_out = open(step->rd->s, O_RDWR | O_CREAT | O_APPEND, 0777);
			if (fd_out == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), step->rd->s);
				exit(1);
			}
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		step->rd = step->rd->next;
	}
	execute(step->cmd, env);
}

int	create_processes(t_step *step, char **env)
{
	int		status;
	int		fds[2];
	int		prev_fd;
	pid_t	pid;
	t_step	*st;
	pid_t	pids[256];
	int		i;
	int		j;

	i = 0;
	st = step;
	prev_fd = -1;
	while (st)
	{
		if (st->pipe && pipe(fds) == -1)
			exit(1);
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid == 0)
			execute_child_process(fds, prev_fd, step, env);
		else
		{
			pids[i] = pid;
			if (prev_fd != -1)
				close(prev_fd);
			if (step->pipe)
			{
				close(fds[1]);
				prev_fd = fds[0];
			}
			else
				prev_fd = -1;
		}
		step = step->next;
		i++;
	}
	j = 0;
	while (j < i)
	{
		waitpid(pids[j], &status, 0);
		j++;
	}
	return (WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;
	t_step	*step;
	int		status;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	if (!env)
		return (-1);
	/* if (is_builtins(argv[1]) == 0)
	{
		execute_builtin(&argv[1], &env);
		export_print(env);
	} */
	step = test_1();
	status = 0;
	if (step->pipe == 1)
		status = create_processes(step, env);
	/* else if (step->pipe == 0)
		status = execute(step); */
	ft_free_strarr(env);
	return (status);
}
