/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_synerr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:22:19 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/10 20:29:43 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	token_synerr(t_token **head, char *err_s)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(err_s, 2);
	ft_putstr_fd("\'\n", 2);
	tk_lstclear(head);
	return (2);
}

void	synerr(t_token **head, t_line *line, char *err_s)
{
	line->exit_no = token_synerr(head, err_s);
	free(line->tmp);
}

void	synerr_quote(t_token **head, t_line *line)
{
	ft_putstr_fd("bash: syntax error, unclosed quote\n", 2);
	tk_lstclear(head);
	line->exit_no = 2;
	free(line->tmp);
}

int	synerr_rd(t_token **head, char *err_s)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(err_s, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	tk_lstclear(head);
	return (2);
}
