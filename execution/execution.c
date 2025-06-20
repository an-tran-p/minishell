/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/20 17:22:03 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_sigint;

void	execute(char **cmd, char **env, t_step *step)
{
	char	*path;

	if (!cmd || !cmd[0])
	{
		ft_free_eve(step, env);
		exit(0);
	}
	if (!access(cmd[0], X_OK))
		path = cmd[0];
	else
		path = find_path(cmd[0], env);
	if (!path || !path[0])
	{
		ft_put_err("command not found", cmd[0], NULL);
		ft_free_eve(step, env);
		exit(127);
	}
	else if (execve(path, cmd, env) == -1)
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

void	exec_single_cmd_child(t_step *step, char ***env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (step->rd)
		handle_rd(step, step, *env);
	if (step->cmd && is_builtins(step->cmd[0]))
		execute_builtin_in_child(step->cmd, env, step, -1);
	else if (step->cmd && is_builtins(step->cmd[0]) == 0)
		execute(step->cmd, *env, step);
}

int	execute_single_cmd(t_step *step, char ***env)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (step->cmd && is_builtins(step->cmd[0]) && !step->rd)
	{
		status = execute_builtin_in_parent(step->cmd, env, step, -1);
		return (status);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			return (-1);
		g_sigint = SIGINT_CHILD;
		// fprintf(stderr, "I set sigint to %d\n", sigint);
		if (pid == 0)
			exec_single_cmd_child(step, env);
		else
			waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		else
			status = WEXITSTATUS(status);
		g_sigint = SIGINT_NONE;
	}
	close_hd(step);
	return (status);
}
