/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/04 23:27:57 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test_cases.c"

void	execute(char **cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd[0])
		exit(0);
	path = find_path(cmd[0], env);
	if (!path)
	{
		ft_put_err("command not found: ", cmd[0]);
		exit(127);
	}
	if (execve(path, cmd, env) == -1)
	{
		free(path);
		perror("Execution fails\n");
		exit(127);
	}
	free(path);
}

void	handle_rd(t_token *redirection)
{
	int		fd_in;
	int		fd_out;
	t_token	*rd;

	rd = redirection;
	while (rd)
	{
		if (rd->type == RD_INFILE)
		{
			fd_in = open(rd->s, O_RDONLY);
			if (fd_in == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), rd->s);
				exit(1);
			}
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		if (rd->type == RD_OUTFILE || rd->type == RD_APPEND)
		{
			if (rd->type == RD_OUTFILE)
				fd_out = open(rd->s, O_RDWR | O_CREAT | O_TRUNC, 0777);
			else if (rd->type == RD_APPEND)
				fd_out = open(rd->s, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (fd_out == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), rd->s);
				exit(1);
			}
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		rd = rd->next;
	}
}

void	execute_child_process(int *fds, int prev_fd, t_step *step, char **env)
{
	int	status;

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
	if (step->rd)
		handle_rd(step->rd);
	ft_printf("executing cmd %s\n", step->cmd[0]);
	if (is_builtins(step->cmd[0]))
	{
		status = execute_builtin(step->cmd, &env);
		ft_free_strarr(env);
		ft_free_step(step);
		exit(status);
	}
	else
		execute(step->cmd, env);
}

int	execute_single_cmd(t_step *step, char ***env)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (is_builtins(step->cmd[0]) && !step->rd)
		return (execute_builtin(step->cmd, env));
	else
	{
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			if (step->rd)
				handle_rd(step->rd);
			if (is_builtins(step->cmd[0]))
				return (execute_builtin(step->cmd, env));
			else if (is_builtins(step->cmd[0]) == 0)
				execute(step->cmd, *env);
		}
		else
			waitpid(pid, &status, 0);
	}
	return (WEXITSTATUS(status));
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
			execute_child_process(fds, prev_fd, st, env);
		pids[i] = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (st->pipe)
		{
			close(fds[1]);
			prev_fd = fds[0];
		}
		else
			prev_fd = -1;
		st = st->next;
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
	t_step	*step;
	int		status;
	char	**env;

	(void)argc;
	(void)argv;
	env = copy_env(envp);
	if (!env)
		return (-1);
	step = test_4();
	status = 0;
	if (step->pipe == 1)
		status = create_processes(step, env);
	else if (step->pipe == 0)
		status = execute_single_cmd(step, &env);
	ft_free_strarr(env);
	ft_free_step(step);
	return (status);
}
