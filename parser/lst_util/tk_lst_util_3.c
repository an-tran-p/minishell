/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_lst_util_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:01:41 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 17:09:13 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tokenizer.h"

char	*get_tk_name(t_etype type)
{
	if (type == WORD_F)
		return ("WORD_F");
	else if (type == WORD)
		return ("WORD");
	else if (type == DEL)
		return ("DEL");
	else if (type == RD_INFILE)
		return ("RD_INFILE");
	else if (type == RD_HDQUOTE)
		return ("RD_HEQUOTE");
	else if (type == RD_HEREDOC)
		return ("RD_HEREDOC");
	else if (type == RD_OUTFILE)
		return ("RD_OUTFILE");
	else if (type == RD_APPEND)
		return ("RD_APPEND");
	else if (type == RD_ERR)
		return ("RD_ERR");
	else
		return ("PIPE");
}

void	tk_lstprint(t_token *head)
{
	if (!head)
		printf("No list exists");
	while (head)
	{
		printf("\x1B[31m""%s""\x1B[0m", head->s);
		printf("(%s) ", get_tk_name(head->type));
		head = head->next;
	}
	printf("\n");
}
