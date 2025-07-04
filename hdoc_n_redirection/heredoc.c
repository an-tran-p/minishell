/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 21:16:45 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
