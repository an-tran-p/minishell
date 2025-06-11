/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:31:42 by ji-hong           #+#    #+#             */
/*   Updated: 2025/05/30 17:58:17 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

void	tk_word_token(t_token **head, t_line *line)
{
	if (line->flag)
		tk_token(head, WORD_F, line);
	else
		tk_token(head, WORD, line);
}

static void	tk_op_token(t_token **head, t_line *line)
{
	if (line->i == 1)
	{
		if (line->c == '|')
			tk_token(head, PIPE, line);
		else if (line->c == '>')
			tk_token(head, RD_OUTFILE, line);
		else if (line->c == '<')
			tk_token(head, RD_INFILE, line);
	}
	else if (line->c != '|' && line->i == 2)
	{
		if (line->c == '>')
			tk_token(head, RD_APPEND, line);
		else if (line->c == '<')
			tk_token(head, RD_HEREDOC, line);
	}
	else
	{
		if (line->c == '|')
			synerr(head, line, "|");
		else if (line->c == '>')
			synerr(head, line, "<");
		else if (line->c == '<')
			synerr(head, line, ">");
	}
}

void	tokenizer_handle_space(t_token **head, t_line *line)
{
	tk_word_token(head, line);
	line->s += trim_space(line->s);
	line->i = -1;
}

void	tokenizer_handle_op(t_token **head, t_line *line)
{
	line->c = line->s[line->i];
	if (line->i && !ft_isspace(line->s[line->i - 1]))
		tk_word_token(head, line);
	line->i = 1;
	while (line->s[line->i] == line->c)
		line->i ++;
	tk_op_token(head, line);
	if (!(*head))
		return ;
	line->s += trim_space(line->s);
	line->i = -1;
}

void	tokenizer_handle_quote(t_token **head, t_line *line)
{
	line->quote = line->s[line->i];
	line->i ++;
	while (line->s[line->i] && line->s[line->i] != line->quote)
		line->i ++;
	if (!line->s[line->i])
	{
		synerr_quote(head, line);
		return ;
	}
	line->flag ++;
}
