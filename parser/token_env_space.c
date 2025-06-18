/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_env_space.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:57:29 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/18 12:57:58 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	token_env_space_1(t_token **cur, int *i, t_env *data)
{
	data->var = data->expand;
	if (*i)
	{
		data->j = 0;
		while (data->var[data->j] && !ft_isspace(data->var[data->j]))
			data->j ++;
		data->s = (char *)ft_calloc(*i + data->j + 1, sizeof(char));
		if (!data->s)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->s, (*cur)->s, *i);
		ft_memcpy(data->s + *i, data->var, data->j);
		data->m_err = tk_appendfront(cur, data->s);
		if (data->m_err)
			return ;
		data->var += data->j;
	}
	while (ft_isspace(*data->var))
	{
		data->var ++;
		data->count --;
	}
}

static void	token_env_space_2(t_token **cur, t_env *data)
{
	while (data->count)
	{
		data->j = 0;
		while (data->var[data->j] && !ft_isspace(data->var[data->j]))
			data->j ++;
		data->s = (char *)ft_calloc(data->j + 1, sizeof(char));
		if (!data->s)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->s, data->var, data->j);
		data->var += data->j;
		while (ft_isspace(*data->var))
		{
			data->var ++;
			data->count --;
		}
		data->m_err = tk_appendfront(cur, data->s);
		if (data->m_err)
			return ;
	}
}

static void	token_env_space_3(t_token **cur, int *i, t_env *data, char *tmp)
{
	data->j = 0;
	while (data->var[data->j] && !ft_isspace(data->var[data->j]))
		data->j ++;
	if (data->j || data->len_tmp)
	{
		data->s = (char *)ft_calloc(data->j + data->len_tmp + 1,
				sizeof(char));
		if (!data->s)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->s, data->var, data->j);
		ft_memcpy(data->s + data->j, data->tmp, data->len_tmp);
		*i = data->j - 1;
		(*cur)->s = data->s;
		free(tmp);
	}
	else if (!data->j)
	{
		(*cur)->s = tmp;
		(*cur)->type = DEL;
	}
}

void	token_env_space(t_token **cur, int *i, t_env *data)
{
	char	*tmp;

	tmp = (*cur)->s;
	token_env_space_1(cur, i, data);
	if (data->m_err)
		return ;
	token_env_space_2(cur, data);
	if (data->m_err)
		return ;
	token_env_space_3(cur, i, data, tmp);
}
