/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 23:25:13 by atran             #+#    #+#             */
/*   Updated: 2025/06/13 18:14:40 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_expand(char **s, char **env)
{
	int	i;
	int	m_err;

	m_err = 0;
	i = 0;
	while ((*s)[i])
	{
		if ((*s)[i] == '$')
		{
			m_err = token_quote_env(s, &i, env);
			if (m_err)
				return (m_err);
		}
		i++;
	}
	return (m_err);
}
