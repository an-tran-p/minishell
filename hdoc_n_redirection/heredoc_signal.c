/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/22 23:21:03 by atran            ###   ########.fr       */
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

int	heredoc_to_skip(char *delimeter, char **env, t_step *step)
{
	char	*line;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (-1);
	g_sigint = SIGINT_HEREDOC;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("minishell: warning: ", 2);
				ft_putstr_fd("here-document delimited by end-of-file ", 2);
				ft_putstr_fd("(wanted '", 2);
				ft_putstr_fd(delimeter, 2);
				ft_putstr_fd("')\n", 2);
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
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			g_sigint = SIGINT_HEREDOC_RECEIVED;
			exit_status(130, true);
			return (-1);
		}
	}
	return (-2);
}

int	last_heredoc(t_token *rd, char **env, t_step *step)
{
	int		fd[2];
	char	*line;
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	g_sigint = SIGINT_HEREDOC;
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("minishell: warning: ", 2);
				ft_putstr_fd("here-document delimited by end-of-file ", 2);
				ft_putstr_fd("(wanted '", 2);
				ft_putstr_fd(rd->s, 2);
				ft_putstr_fd("')\n", 2);
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
	else
	{
		close(fd[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
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
	int		fd_skip;
	t_token	*rd;
	int		hd_num;

	rd = redirection;
	hd_num = count_hdoc(redirection);
	fd_skip = -2;
	fd_in = -2;
	while (rd)
	{
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
		{
			hd_num--;
			if (hd_num > 0)
			{
				fd_skip = heredoc_to_skip(rd->s, env, step);
				if (fd_skip == -1)
					return (-1);
			}
			else if (hd_num == 0)
			{
				fd_in = last_heredoc(rd, env, step);
				fprintf(stderr, "fd of this heredoc is %d\n", fd_in);
				return (fd_in);
			}
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
			st = step;
			while (st)
			{
				if (st->hd_fd >= 0)
					close(st->hd_fd);
				st = st->next;
			}
			return ;
		}
		st = st->next;
	}
}
