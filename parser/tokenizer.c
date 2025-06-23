/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 16:47:44 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	create_tokens(t_token **head, t_line *line)
{
	while (line->s[line->i])
	{
		if (ft_isspace(line->s[line->i]))
			tokenizer_handle_space(head, line);
		else if (line->s[line->i] == '|' || line->s[line->i] == '<'
			|| line->s[line->i] == '>')
			tokenizer_handle_op(head, line);
		else if (line->s[line->i] == '\'' || line->s[line->i] == '\"')
			tokenizer_handle_quote(head, line);
		else if (line->s[line->i] == '$')
			line->flag ++;
		if (line->exit_no)
			return ;
		line->i ++;
	}
	if (line->s[0])
		tk_word_token(head, line);
}

static void	init_t_line(t_line *line, char *s)
{
	line->i = 0;
	line->flag = 0;
	line->exit_no = 0;
	line->c = 0;
	line->quote = 0;
	line->s = s;
	line->tmp = s;
}

static int	tokenizer(t_token **head, char *s)
{
	t_line	line;

	init_t_line(&line, s);
	line.s += trim_space(line.s);
	if (!line.s[0])
		return (0);
	create_tokens(head, &line);
	if (line.exit_no)
		return (line.exit_no);
	return (0);
}

static int	chk_tokens(t_token **head, char **env)
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
		if (tmp->type != DEL)
			tmp->type = WORD;
	}
	return (exit_no);
}

int	parser(char *s, t_step **lst, char **env)
{
	int		exit_no;
	t_token	*head;

	head = NULL;
	exit_no = tokenizer(&head, s);
	if (!head || exit_no)
		return (exit_no);
tk_lstprint(head);
	exit_no = chk_tokens(&head, env);
	if (head)
	{
tk_lstprint(head);
		st_step(&head, lst);
st_lstprint(*lst);
	}
	return (exit_no);
}
