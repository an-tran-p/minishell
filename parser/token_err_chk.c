/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_err_chk.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/13 14:45:36 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	type_isrd(t_etype type)
{
	if (type == RD_INFILE || type == RD_HEREDOC || type == RD_OUTFILE
		|| type == RD_APPEND || type == RD_HDQUOTE || type == RD_ERR)
		return (1);
	else
		return (0);
}

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
//expand check ' ': RD_ERR
		{
			exit_no = token_flag(head, tmp->next, 1, env);
			if(tmp->next->type == RD_ERR)
			{
				tmp->type= RD_ERR;
				free(tmp->s);
				tmp->s= tmp->next->s;
				t_token *p;
				p = tmp->next;
				tmp->next= tmp->next->next;
				free(p);
				synerr_rd(head, tmp->s);
				return(2);
			}
			tmp = tmp->next;
		}
	}
printf("exit_no_%d\n", exit_no);
	return (exit_no);
}

static t_token	*chk_tokens_loop(t_token **head, int *exit_no, char **env)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp && tmp->next)
	{
		if (tmp->type == WORD_F)
		{
			*exit_no = token_flag(head, tmp, 0, env);
			if(!(*exit_no) && tmp->type != DEL)
				tmp->type = WORD;
		}
		else if (type_isrd(tmp->type))
		{
			*exit_no = chk_rd_err(head, tmp, env);
			if (*exit_no)
				return (NULL) ;
			free(tmp->s);
			tmp->s = tmp->next->s;
			tmp->next->s = NULL;
			tmp->next->type = DEL;
		}
		else if (tmp->type == PIPE && tmp->next->type == PIPE)
		{
			*exit_no=token_synerr(head, tmp->s);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (tmp);
}

int	chk_tokens(t_token **head, char **env)
{
	int		exit_no;
	t_token	*tmp;

	exit_no = 0;
	tmp = *head;
	if (tmp->type == PIPE)
	{
		exit_no = token_synerr(head, "|");
		return (exit_no);
	}
	tmp = chk_tokens_loop(head, &exit_no, env);
	if (!tmp)
		return (exit_no);
	if (tmp && (tmp->type == PIPE || type_isrd(tmp->type)))
	{
		exit_no = token_synerr(head, "newline");
	}
	else if (tmp && tmp->type == WORD_F)
	{
		token_flag(head, tmp, 0, env);
		if(tmp->type != DEL)
			tmp->type = WORD;
	}
	return (exit_no);
}
