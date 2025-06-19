/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_get_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:30:53 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/19 20:06:26 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	token_expand_str(char **str, int *i, t_env *data)
{
	data->len_expand = ft_strlen(data->expand);
	data->s = (char *)ft_calloc((*i) + data->len_expand + data->len_tmp + 1,
			sizeof(char));
	if (!data->s)
		return (-1);
	ft_memcpy(data->s, *str, *i);
	ft_memcpy(data->s + *i, data->expand, data->len_expand);
	ft_memcpy(data->s + *i + data->len_expand, data->tmp,
		data->len_tmp + 1);
	free(*str);
	data->tmp = NULL;
	*i = *i + data->len_expand - 1;
	*str = data->s;
	if (data->ex_free)
		free(data->expand);
	return (0);
}

static int	chk_envchr(int c)
{
	if (c == '_' || ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static int	token_get_envname(t_env *data, char *s)
{
	data->j = 0;
	data->ex_free = 0;
	if (s[0] == '?')
	{
		data->expand = ft_itoa(exit_status(0, false));
		if (!data->expand)
			return (-1);
		data->ex_free = 1;
		data->j = 1;
	}
	else
	{
		while (chk_envchr(s[data->j]))
		{
			if (ft_isdigit(s[0]))
			{
				data->j = 1;
				break ;
			}
			data->j ++;
		}
	}
	return (0);
}

void	token_get_env(char **str, int *i, t_env *data, char **env)
{
	data->m_err = token_get_envname(data, (*str) + (*i) + 1);
	if (data->m_err || !data->j)
		return ;
	else if (data->j)
	{
		data->var = (char *)ft_calloc(data->j + 1, sizeof(char));
		if (!data->var)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->var, (*str) + (*i) + 1, data->j);
	}
	if (!data->ex_free)
		data->expand = ft_getenv(env, data->var);
	free(data->var);
	data->tmp = (*str) + (*i) + data->j + 1;
	data->len_tmp = ft_strlen(data->tmp);
	if (!data->expand || !data->expand[0])
		ft_memcpy((*str) + (*i), data->tmp, data->len_tmp + 1);
}
