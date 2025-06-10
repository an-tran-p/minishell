/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/10 21:37:23 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	fprintf(stderr, "I am counting %d heredocs\n", num);
	return (num);
}

void	heredoc_to_skip(char *delimeter)
{
	char	*line;

	fprintf(stderr, "this is heredoc to skip\n");
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
}

int	last_heredoc(char *delimeter)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	fprintf(stderr, "I have reached the last heredoc\n");
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	heredoc_in_step(t_token *redirection)
{
	int		fd_in;
	t_token	*rd;
	int		hd_num;

	rd = redirection;
	hd_num = count_hdoc(redirection);
	fd_in = 0;
	while (rd)
	{
		fprintf(stderr, "rd type is %d\n", rd->type);
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
		{
			hd_num--;
			if (hd_num > 0)
				heredoc_to_skip(rd->s);
			else if (hd_num == 0)
			{
				fd_in = last_heredoc(rd->s);
				return (fd_in);
			}
		}
		rd = rd->next;
	}
	return (fd_in);
}

void	handle_heredoc(t_step *step)
{
	t_step	*st;

	st = step;
	if (!st)
		return ;
	while (st)
	{
		st->hd_fd = heredoc_in_step(st->rd);
		if (st->hd_fd == -1)
		{
			st = step;
			while (st)
			{
				if (st->hd_fd > 0)
					close(st->hd_fd);
				st = st->next;
			}
			return ;
		}
		st = st->next;
	}
}
