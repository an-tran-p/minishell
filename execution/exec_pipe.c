/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 22:38:46 by atran            ###   ########.fr       */
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
	g_sigint = SIGINT_NONE;
	return (f_status);
}

void	execute_child_process(t_process *pro)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (pro->prev_fd != -1 && !have_infile(pro->st->rd) && pro->st->hd_fd < 0)
		dup2(pro->prev_fd, STDIN_FILENO);
	if (pro->st->pipe && !have_outfile(pro->st->rd))
		dup2(pro->fds[1], STDOUT_FILENO);
	if (pro->prev_fd != -1)
		close(pro->prev_fd);
	if (pro->st->pipe)
	{
		if (!pro->st->cmd || (pro->st->cmd && !is_builtins(pro->st->cmd[0])))
			close(pro->fds[0]);
		close(pro->fds[1]);
	}
	if (pro->st->rd)
		handle_rd(pro->st, pro->step, pro->env);
	close_hd(pro->step);
	if (pro->st->cmd && is_builtins(pro->st->cmd[0]))
		execute_builtin_in_child(pro->st->cmd, &pro->env, pro->step,
			pro->fds[0]);
	if (pro->st->cmd && !is_builtins(pro->st->cmd[0]))
		execute(pro->st->cmd, pro->env, pro->step);
	ft_free_eve(pro->step, pro->env);
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

void	process_loop(t_process *pro, int *i)
{
	while (pro->st)
	{
		if (pro->st->pipe)
		{
			if (pipe(pro->fds) == -1)
			{
				pro->pid = -1;
				break ;
			}
		}
		pro->pid = fork();
		if (pro->pid == -1)
			break ;
		g_sigint = SIGINT_CHILD;
		if (pro->pid == 0)
			execute_child_process(pro);
		parent_process(&pro->prev_fd, pro->fds, pro->st, pro->pid);
		pro->st = pro->st->next;
		(*i)++;
	}
}

int	create_processes(t_step *step, char **env)
{
	t_process	pro;
	int			i;

	pro.prev_fd = -1;
	pro.st = step;
	pro.step = step;
	pro.env = env;
	i = 0;
	process_loop(&pro, &i);
	if (pro.pid == -1)
		parent_process(&pro.prev_fd, pro.fds, pro.st, pro.pid);
	return (process_wait(pro.pid, i));
}
