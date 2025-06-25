/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 15:35:03 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	token_quote_env(char **str, int *i, char **env)
{
	t_env	data;

	token_get_env(str, i, &data, env);
	if (!data.j)
		return (data.m_err);
	if (data.m_err || !data.expand || !data.expand[0])
	{
		if (!data.expand ||!data.expand[0])
		{
			ft_memcpy((*str) + (*i), data.tmp, data.len_tmp + 1);
			(*i)--;
		}
		return (data.m_err);
	}
	data.m_err = token_expand_str(str, i, &data);
	return (data.m_err);
}

int	token_quote(char **str, int *i, int heredoc, char **env)
{
	int		m_err;
	int		len;
	char	quote;
	char	*tmp;

	quote = (*str)[*i];
	tmp = (*str) + (*i) + 1;
	len = ft_strlen(tmp);
	ft_memcpy((*str) + *i, tmp, len + 1);
	while ((*str)[*i] != quote)
	{
		if (!heredoc && quote == '\"' && (*str)[*i] == '$')
		{
			m_err = token_quote_env(str, i, env);
			if (m_err)
				return (m_err);
		}
		(*i)++;
	}
	tmp = (*str) + (*i) + 1;
	len = ft_strlen(tmp);
	ft_memcpy((*str) + *i, tmp, len + 1);
	(*i)--;
	return (0);
}

int	token_flag(t_token **head, t_token *cur, int rd, char **env)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (cur->s[i])
	{
		if (cur->s[i] == '\'' || cur->s[i] == '\"')
		{
			if (token_quote(&cur->s, &i, 0, env))
				m_err_exit_token(head);
		}
		else if (cur->s[i] == '$')
		{
			err = token_env(&cur, &i, rd, env);
			if (err == -1)
				m_err_exit_token(head);
		}
		i++;
	}
	return (err);
}
