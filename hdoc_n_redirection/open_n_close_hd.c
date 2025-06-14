/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_n_close_hd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:25:13 by atran             #+#    #+#             */
/*   Updated: 2025/06/15 00:06:36 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_hd_fd(t_step *step)
{
	t_step	*st;

	if (!step)
		return ;
	st = step;
	while (st)
	{
		st->hd_fd = -2;
		st = st->next;
	}
}

void	close_hd(t_step *step)
{
	t_step	*st;

	st = step;
	fprintf(stderr, "I am closing all hd_fd\n");
	while (st)
	{
		if (st->hd_fd != -1 && st->hd_fd != -2)
		{
			close(st->hd_fd);
			st->hd_fd = -2;
		}
		st = st->next;
	}
}
