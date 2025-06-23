/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:28:04 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 15:39:04 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static int	token_env_type(t_token **cur, int rd)
{
	if (rd)
	{
		(*cur)->type = RD_ERR;
		return (1);
	}
	else
		(*cur)->type = DEL;
	return (0);
}

int	token_env(t_token **cur, int *i, int rd, char **env)
{
	t_env	data;

	token_get_env(&(*cur)->s, i, &data, env);
	if (!data.j)
		return (data.m_err);
	if (data.m_err || !data.expand || !data.expand[0])
	{
		if (!data.expand || !data.expand[0])
		{
			if (!(*i) && !data.len_tmp)
			{
				if (token_env_type(cur, rd))
					return (1);
			}
			ft_memcpy((*cur)->s + (*i), data.tmp, data.len_tmp + 1);
			(*i)--;
		}
		return (data.m_err);
	}
	token_env_expansion(cur, i, &data, rd);
	return (data.m_err);
}
