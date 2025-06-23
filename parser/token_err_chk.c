/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_err_chk.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 17:12:24 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	token_flag_heredoc(t_token *cur, char **env)
{
	int	i;

	i = 0;
	while (cur->next->s[i])
	{
		if (cur->next->s[i] == '\'' || cur->next->s[i] == '\"')
		{
			token_quote(&cur->next->s, &i, 1, env);
			cur->type = RD_HDQUOTE;
		}
		i ++;
	}
	cur->next->type = WORD;
	cur = cur->next;
}

static int	token_flag_rd_err(t_token **head, t_token *tmp)
{
	t_token	*p;

	if (tmp->next->type == RD_ERR)
	{
		tmp->type = RD_ERR;
		free(tmp->s);
		tmp->s = tmp->next->s;
		p = tmp->next;
		tmp->next = tmp->next->next;
		free(p);
		synerr_rd(head, tmp->s);
		return (2);
	}
	return (0);
}

static int	chk_rd_err(t_token **head, t_token *tmp, char **env)
{
	int	exit_no;

	exit_no = 0;
	if (tmp->next->type != WORD && tmp->next->type != WORD_F)
	{
		token_synerr(head, tmp->next->s);
		return (2);
	}
	if (tmp->next->type == WORD_F)
	{
		if (tmp->type == RD_HEREDOC)
			token_flag_heredoc(tmp, env);
		else
		{
			exit_no = token_flag(head, tmp->next, 1, env);
			exit_no = token_flag_rd_err(head, tmp);
			if (exit_no)
				return (2);
			tmp = tmp->next;
		}
	}
	return (exit_no);
}

static void	token_type_rd(t_token **head, t_token **tmp, int *exit_no,
		char **env)
{
	*exit_no = chk_rd_err(head, *tmp, env);
	if (*exit_no)
		return ;
	free((*tmp)->s);
	(*tmp)->s = (*tmp)->next->s;
	(*tmp)->next->s = NULL;
	(*tmp)->next->type = DEL;
}

t_token	*chk_tokens_loop(t_token **head, int *exit_no, char **env)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp && tmp->next)
	{
		if (tmp->type == WORD_F)
		{
			*exit_no = token_flag(head, tmp, 0, env);
			if (!(*exit_no) && tmp->type != DEL)
				tmp->type = WORD;
		}
		else if (type_isrd(tmp->type))
		{
			token_type_rd(head, &tmp, exit_no, env);
			if (*exit_no)
				return (NULL);
		}
		else if (tmp->type == PIPE && tmp->next->type == PIPE)
		{
			*exit_no = token_synerr(head, tmp->s);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (tmp);
}
