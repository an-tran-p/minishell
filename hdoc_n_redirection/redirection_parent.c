/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parent.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/26 10:56:19 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_infile_parent(t_token *rd)
{
	int	fd_in;

	fd_in = open(rd->s, O_RDONLY);
	if (fd_in == -1)
	{
		ft_put_err(strerror(errno), rd->s, NULL);
		return (1);
	}
	close(fd_in);
	return (0);
}

int	handle_outfile_parent(t_token *rd)
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
		return (1);
	}
	close(fd_out);
	return (0);
}

int	handle_rd_parent(t_step *step)
{
	t_token	*rd;
	t_step	*st;

	st = step;
	rd = st->rd;
	if (!rd)
		return (0);
	if (st->hd_fd != -1 && st->hd_fd != -2)
		close(st->hd_fd);
	while (rd)
	{
		if (rd->type == RD_OUTFILE || rd->type == RD_APPEND)
		{
			if (handle_outfile_parent(rd))
				return (1);
		}
		if (rd->type == RD_INFILE)
		{
			if (handle_infile_parent(rd))
				return (1);
		}
		rd = rd->next;
	}
	return (0);
}
