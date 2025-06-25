/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 21:02:38 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	infile_last(t_token *redirection)
{
	int		flag;
	t_token	*rd;

	flag = 0;
	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_INFILE)
			flag = 1;
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
			flag = 2;
		rd = rd->next;
	}
	return (flag);
}

void	handle_infile(t_token *rd, t_step *st, t_step *step, char **env)
{
	int	fd_in;

	fd_in = open(rd->s, O_RDONLY);
	if (fd_in == -1)
	{
		ft_put_err(strerror(errno), rd->s, NULL);
		close_hd(step);
		ft_free_eve(step, env);
		exit(1);
	}
	else if (infile_last(st->rd) == 1 && st->cmd)
		dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

void	handle_outfile(t_token *rd, t_step *st, t_step *step, char **env)
{
	int	fd_out;

	fd_out = -2;
	if (rd->type == RD_OUTFILE)
		fd_out = open(rd->s, O_RDWR | O_CREAT | O_TRUNC, 0777);
	else if (rd->type == RD_APPEND)
		fd_out = open(rd->s, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd_out == -1)
	{
		ft_put_err(strerror(errno), rd->s, NULL);
		close_hd(step);
		ft_free_eve(step, env);
		exit(1);
	}
	if (st->cmd)
		dup2(fd_out, STDOUT_FILENO);
	if (fd_out > 0)
		close(fd_out);
}

void	handle_rd(t_step *st, t_step *step, char **env)
{
	t_token	*rd;

	rd = st->rd;
	if (!rd)
		return ;
	if (st->hd_fd != -1 && st->hd_fd != -2)
	{
		dup2(st->hd_fd, STDIN_FILENO);
		close(st->hd_fd);
	}
	while (rd)
	{
		if (rd->type == RD_OUTFILE || rd->type == RD_APPEND)
			handle_outfile(rd, st, step, env);
		if (rd->type == RD_INFILE)
			handle_infile(rd, st, step, env);
		rd = rd->next;
	}
	close_hd(step);
}
