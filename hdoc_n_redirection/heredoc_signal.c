/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/23 18:37:57 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_sigint;

int	count_hdoc(t_token *redirection)
{
	int		num;
	t_token	*rd;

	num = 0;
	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
			num++;
		rd = rd->next;
	}
	return (num);
}

void	ft_put_warning_eof(char *s)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file ", 2);
	ft_putstr_fd("(wanted '", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("')\n", 2);
}

void	sigint_heredoc_clean(char **env, t_step *step, char **line)
{
	ft_free_str(line);
	close_hd(step);
	ft_free_eve(step, env);
	exit(130);
}

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

void	last_heredoc_child(int *fd, t_token *rd, char **env, t_step *step)
{
	char	*line;

	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
	close(fd[0]);
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
		{
			ft_put_warning_eof(rd->s);
			break ;
		}
		if (ft_strcmp(line, rd->s) == 0)
		{
			ft_free_str(&line);
			break ;
		}
		if (rd->type == RD_HEREDOC)
		{
			if (heredoc_expand(&line, env))
				break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
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

int	heredoc_in_step(t_token *redirection, char **env, t_step *step)
{
	int		fd_in;
	t_token	*rd;
	int		hd_num;

	rd = redirection;
	hd_num = count_hdoc(redirection);
	fd_in = -2;
	while (rd)
	{
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
		{
			hd_num--;
			if (hd_num > 0)
			{
				if (heredoc_to_skip(rd->s, env, step) == -1)
					return (-1);
			}
			else if (hd_num == 0)
				return (last_heredoc(rd, env, step));
		}
		rd = rd->next;
	}
	return (fd_in);
}

void	handle_heredoc(t_step *step, char **env)
{
	t_step	*st;

	st = step;
	if (!st)
		return ;
	while (st)
	{
		st->hd_fd = heredoc_in_step(st->rd, env, step);
		if (st->hd_fd == -1)
		{
			close_hd(step);
			return ;
		}
		st = st->next;
	}
}
