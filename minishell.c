/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/10 21:55:07 by atran            ###   ########.fr       */
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

int	have_infile(t_token *redirection)
{
	t_token	*rd;

	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_INFILE || rd->type == RD_HDQUOTE
			|| rd->type == RD_HEREDOC)
			return (1);
		rd = rd->next;
	}
	return (0);
}

int	have_outfile(t_token *redirection)
{
	t_token	*rd;

	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_OUTFILE || rd->type == RD_APPEND)
			return (1);
		rd = rd->next;
	}
	return (0);
}

void	execute_child_process(int *fds, int prev_fd, t_step *step, char **env)
{
	int	status;

	if (prev_fd != -1 && !have_infile(step->rd) && step->hd_fd <= 0)
		dup2(prev_fd, STDIN_FILENO);
	if (step->pipe && !have_outfile(step->rd))
		dup2(fds[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (step->pipe)
	{
		close(fds[0]);
		close(fds[1]);
	}
	if (step->rd)
		handle_rd(step);
	if (is_builtins(step->cmd[0]))
	{
		status = execute_builtin(step->cmd, &env);
		ft_free_strarr(env);
		ft_free_step(step);
		exit(status);
	}
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
				handle_rd(step);
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
		if (st->hd_fd > 0)
			close(st->hd_fd);
		if (st->pipe)
		{
			close(fds[1]);
			prev_fd = fds[0];
		}
		else if (!st->pipe)
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
	step = test_5();
	status = 0;
	handle_heredoc(step);
	if (step->pipe == 1)
		status = create_processes(step, env);
	else if (step->pipe == 0)
		status = execute_single_cmd(step, &env);
	ft_free_strarr(env);
	ft_free_step(step);
	return (status);
}
