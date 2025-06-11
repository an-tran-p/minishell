/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/11 19:22:09 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	token_quote_env(char **str, int *i)
{
	t_env	env;

	token_get_env(str, i, &env);
	if (env.m_err || !env.j || !env.expand || !env.expand[0])
		return (env.m_err);
	env.m_err = token_expand_str(str, i, &env);
	return (env.m_err);
}

int	token_quote(char **str, int *i, int heredoc)
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
			m_err = token_quote_env(str, i);
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

int	token_flag(t_token **head, t_token *cur, int rd)
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	while (cur->s[i])
	{
		if (cur->s[i] == '\'' || cur->s[i] == '\"')
			if (token_quote(&cur->s, &i, 0))
				m_err_exit_token(head);
		if (cur->s[i] == '$')
		{
			err = token_env(&cur, &i, rd);
			if (err == -1)
				m_err_exit_token(head);
/*
			else if (err == 1)
			{
				err = synerr_rd(head, cur->s);
				return (err);
			}
*/
		}
		i ++;
	}
	return (err);
}
