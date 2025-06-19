/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:28:04 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/19 20:06:55 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

/*
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
*/

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

static int	token_env_expansion(t_token **cur, int *i, t_env *data, int rd)
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
				if (rd)
				{
					(*cur)->type = RD_ERR;
					return (1);
				}
				else
					(*cur)->type = DEL;
			}
			(*i)--;
		}
		return (data.m_err);
	}
	token_env_expansion(cur, i, &data, rd);
	return (data.m_err);
}

/*
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
				if (rd)
				{
					(*cur)->type = RD_ERR;
					return (1);
				}
				else
					(*cur)->type = DEL;
			}
			(*i)--;
		}
		return (data.m_err);
	}
	data.count = chk_env_space(data.expand);
	if (rd && data.count)
	{
		(*cur)->type = RD_ERR;
		return (1);
	}
	if (!data.count)
		data.m_err = token_expand_str(&(*cur)->s, i, &data);
	else if (data.count == -1)
		token_env_all_space(cur, i, &data);
	else
		token_env_space(cur, i, &data);
	return (data.m_err);
}
*/
