/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_env_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:28:04 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 15:40:16 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	token_env_all_space_back(t_token **cur, int *i, t_env *data)
{
	if (data->len_tmp)
	{
		if (!(*i))
		{
			ft_memcpy((*cur)->s, data->tmp, data->len_tmp + 1);
			return ;
		}
		data->s = (char *)ft_calloc(data->len_tmp + 1, sizeof(char));
		if (!data->s)
		{
			data->m_err = -1;
			return ;
		}
		ft_memcpy(data->s, data->tmp, data->len_tmp);
		*i = 0;
		(*cur)->s = data->s;
	}
}

static void	token_env_all_space(t_token **cur, int *i, t_env *data)
{
	if (!(*i) && !data->len_tmp)
		(*cur)->type = DEL;
	if (*i)
	{
		(*cur)->s[*i] = 0;
		if (!data->len_tmp)
			return ;
		data->m_err = tk_appendfront(cur, (*cur)->s);
		if (data->m_err)
			return ;
	}
	token_env_all_space_back(cur, i, data);
}

static int	chk_env_space(char *s)
{
	int	i;
	int	len;
	int	count;

	i = 0;
	len = ft_strlen(s);
	count = 0;
	while (s[i])
	{
		if (ft_isspace(s[i]))
			count ++;
		i ++;
	}
	if (count == len)
		return (-1);
	return (count);
}

int	token_env_expansion(t_token **cur, int *i, t_env *data, int rd)
{
	data->count = chk_env_space(data->expand);
	if (rd && data->count)
	{
		(*cur)->type = RD_ERR;
		return (1);
	}
	if (!data->count)
		data->m_err = token_expand_str(&(*cur)->s, i, data);
	else if (data->count == -1)
		token_env_all_space(cur, i, data);
	else
		token_env_space(cur, i, data);
	return (data->m_err);
}
