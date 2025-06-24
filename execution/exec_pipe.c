/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 21:18:33 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_sigint;

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

void	execute_child_process(int *fds, int prev_fd, t_step *st, t_step *step,
		char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1 && !have_infile(st->rd) && st->hd_fd < 0)
		dup2(prev_fd, STDIN_FILENO);
	if (st->pipe && !have_outfile(st->rd))
		dup2(fds[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (st->pipe)
	{
		if (!st->cmd || (st->cmd && !is_builtins(st->cmd[0])))
			close(fds[0]);
		close(fds[1]);
	}
	if (st->rd)
		handle_rd(st, step, env);
	close_hd(step);
	if (st->cmd && is_builtins(st->cmd[0]))
		execute_builtin_in_child(st->cmd, &env, step, fds[0]);
	if (st->cmd && !is_builtins(st->cmd[0]))
		execute(st->cmd, env, step);
	ft_free_eve(step, env);
	exit(0);
}

void	parent_process(int *prev_fd, int fds[2], t_step *st, pid_t pid)
{
	if (*prev_fd >= 0)
	{
		close(*prev_fd);
		*prev_fd = -1;
	}
	if (st->pipe && pid != -1)
	{
		close(fds[1]);
		*prev_fd = fds[0];
	}
	else
	{
		close(fds[0]);
		close(fds[1]);
		*prev_fd = -1;
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
		g_sigint = SIGINT_CHILD;
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
