/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 22:58:16 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_sigint;

void	heredoc_to_skip_child(char *delimeter, char **env, t_step *step)
{
	char	*line;

	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		rl_event_hook = sig_hook;
		line = readline("> ");
		if (g_sigint == SIGINT_HEREDOC_RECEIVED)
			sigint_heredoc_clean(env, step, &line);
		if (!line)
		{
			ft_put_warning_eof(delimeter);
			break ;
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
	close_hd(step);
	ft_free_eve(step, env);
	exit(0);
}

int	heredoc_to_skip(char *delimeter, char **env, t_step *step)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (-1);
	g_sigint = SIGINT_HEREDOC;
	if (pid == 0)
		heredoc_to_skip_child(delimeter, env, step);
	else
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == 130)
		{
			g_sigint = SIGINT_HEREDOC_RECEIVED;
			exit_status(130, true);
			return (-1);
		}
	}
	return (-2);
}

void	readline_heredoc(int *fd, t_token *rd, char **env, t_step *step)
{
	char	*line;

	while (1)
	{
		rl_event_hook = sig_hook;
		line = readline("> ");
		if (g_sigint == SIGINT_HEREDOC_RECEIVED)
		{
			close(fd[1]);
			sigint_heredoc_clean(env, step, &line);
		}
		if (!line)
			return (ft_put_warning_eof(rd->s));
		if (ft_strcmp(line, rd->s) == 0)
			return (ft_free_str(&line));
		if (rd->type == RD_HEREDOC)
		{
			if (heredoc_expand(&line, env))
				return ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}

void	last_heredoc_child(int *fd, t_token *rd, char **env, t_step *step)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	close(fd[0]);
	readline_heredoc(fd, rd, env, step);
	close(fd[1]);
	close_hd(step);
	ft_free_eve(step, env);
	exit(0);
}

int	last_heredoc(t_token *rd, char **env, t_step *step)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	g_sigint = SIGINT_HEREDOC;
	if (pid == 0)
		last_heredoc_child(fd, rd, env, step);
	else
	{
		close(fd[1]);
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == 130)
		{
			close(fd[0]);
			g_sigint = SIGINT_HEREDOC_RECEIVED;
			exit_status(130, true);
			return (-1);
		}
	}
	return (fd[0]);
}
