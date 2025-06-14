/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/14 23:22:39 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_rd(t_token *rd)
{
	t_token	*tmp;

	while (rd)
	{
		tmp = rd->next;
		free(rd->s);
		free(rd);
		rd = tmp;
	}
}

void	ft_free_step(t_step *step)
{
	t_step	*tmp;

	while (step)
	{
		tmp = step->next;
		if (step->rd)
			free_rd(step->rd);
		if (step->cmd)
			ft_free_strarr(step->cmd);
		free(step);
		step = tmp;
	}
}

void	ft_free_eve(t_step *step, char **env)
{
	ft_free_strarr(env);
	ft_free_step(step);
}
