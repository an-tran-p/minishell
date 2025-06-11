/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/11 23:21:17 by atran            ###   ########.fr       */
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
		ft_put_err(" :command not found", cmd[0]);
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

void	execute_child_process(int *fds, int prev_fd, t_step *st, t_step *step,
		char **env)
{
	int	status;

	fprintf(stderr, "PID: %d executing command: %s\n", getpid(), st->cmd[0]);
	if (prev_fd != -1 && !have_infile(st->rd) && st->hd_fd <= 0)
		dup2(prev_fd, STDIN_FILENO);
	if (st->pipe && !have_outfile(st->rd))
		dup2(fds[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (st->pipe)
	{
		close(fds[0]);
		close(fds[1]);
	}
	if (st->rd)
		handle_rd(st);
	if (is_builtins(st->cmd[0]))
	{
		status = execute_builtin(st->cmd, &env);
		ft_free_strarr(env);
		ft_free_step(step);
		exit(status);
	}
	execute(st->cmd, env);
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
			return (-1);
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

int	process_wait(pid_t pid, int i)
{
	int	j;
	int	status;
	int	f_status;
	int	wait;

	j = 0;
	f_status = 0;
	while (j < i)
	{
		wait = waitpid(-1, &status, 0);
		if (wait == pid)
		{
			if (WIFSIGNALED(status))
				f_status = WTERMSIG(status) + 128;
			else
				f_status = WEXITSTATUS(status);
		}
		if (wait == -1)
			return (-1);
		j++;
	}
	return (f_status);
}

void	parent_process(int *prev_fd, int fds[2], t_step *st, pid_t pid)
{
	if (*prev_fd != -1)
	{
		close(*prev_fd);
		*prev_fd = -1;
	}
	if (st->hd_fd != -1)
	{
		close(st->hd_fd);
		st->hd_fd = -1;
	}
	if (st->pipe && pid != -1)
	{
		close(fds[1]);
		*prev_fd = fds[0];
	}
	if (!st->pipe || pid == -1)
	{
		*prev_fd = -1;
		close(fds[0]);
		close(fds[1]);
	}
}

int	create_processes(t_step *step, char **env)
{
	int		fds[2];
	int		prev_fd;
	pid_t	pid;
	t_step	*st;
	int		i;

	i = 0;
	st = step;
	prev_fd = -1;
	while (st)
	{
		if (st->pipe)
		{
			if (pipe(fds) == -1)
			{
				pid = -1;
				break ;
			}
		}
		pid = fork();
		if (pid == -1)
			break ;
		if (pid == 0)
			execute_child_process(fds, prev_fd, st, step, env);
		parent_process(&prev_fd, fds, st, pid);
		st = st->next;
		i++;
	}
	if (pid == -1)
		parent_process(&prev_fd, fds, st, pid);
	return (process_wait(pid, i));
}

/* int	main(int argc, char **argv, char **envp)
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
	handle_heredoc(step);q
	if (step->pipe == 1)
		status = create_processes(step, env);
	else if (step->pipe == 0)
		status = execute_single_cmd(step, &env);
	ft_free_strarr(env);
	ft_free_step(step);
	return (status);
} */
