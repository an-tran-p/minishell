/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/19 20:52:44 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	sigint;

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

int	heredoc_to_skip(char *delimeter)
{
	char	*line;

	signal(SIGINT, handling_sigint_heredoc);
	while (1)
	{
		line = readline("> ");
		if (sigint)
		{
			ft_free_str(&line);
			fprintf(stderr, "I am here");
			signal(SIGINT, sigint_parent_handler);
			return (-3);
		}
		if (!line)
			break ;
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}

int	last_heredoc(t_token *rd, char **env)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	rl_catch_signals = 0;
	signal(SIGINT, handling_sigint_heredoc);
	sigint = 0;
	while (1)
	{
		line = readline("> ");
		fprintf(stderr, "out of condition: heredoc_interupted is %d\n", sigint);
		if (sigint)
		{
			fprintf(stderr, "heredoc_interupted is %d\n", sigint);
			close(fd[0]);
			close(fd[1]);
			ft_free_str(&line);
			fprintf(stderr, "I am trying to end heredoc\n");
			signal(SIGINT, sigint_parent_handler);
			return (-3);
		}
		if (!line || ft_strcmp(line, rd->s) == 0)
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
	return (fd[0]);
}

int	heredoc_in_step(t_token *redirection, char **env)
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
				if (heredoc_to_skip(rd->s) == -3)
					return (-3);
			}
			else if (hd_num == 0)
			{
				fd_in = last_heredoc(rd, env);
				fprintf(stderr, "fd of this heredoc is %d\n", fd_in);
				return (fd_in);
			}
		}
		rd = rd->next;
	}
	return (fd_in);
}

int	handle_heredoc(t_step *step, char **env)
{
	t_step	*st;

	st = step;
	if (!st)
		return (0);
	while (st)
	{
		st->hd_fd = heredoc_in_step(st->rd, env);
		if (st->hd_fd == -1 || st->hd_fd == -3)
		{
			st = step;
			while (st)
			{
				if (st->hd_fd >= 0)
					close(st->hd_fd);
				st = st->next;
			}
			if (st->hd_fd == -3)
				return (-3);
			return (0);
		}
		st = st->next;
	}
	return (0);
}

/* int	last_herdoc(t_token *rd, char **env)
{
	int		fd[2];
	char	*line;
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{

	}
} */
