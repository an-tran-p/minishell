/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_lst_util_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:34:32 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/04 17:02:10 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tokenizer.h"

t_token	*tk_lstnew(t_etype type, char *s)
{
	t_token	*new_node;

	new_node = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->s = s;
	return (new_node);
}

void	tk_lstadd_front(t_token **head, t_token *new_node)
{
	if (new_node)
	{
		new_node->next = *head;
		*head = new_node;
	}
}

/*
int	tk_lstsize(t_token *head)
{
	int	i;

	i = 0;
	while (head)
	{
		head = head->next;
		i ++;
	}
	return (i);
}
*/

t_token	*tk_lstlast(t_token *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	tk_lstadd_back(t_token **head, t_token *new_node)
{
	t_token	*tmp;

	if (!*head)
		*head = new_node;
	else if (new_node)
	{
		tmp = tk_lstlast(*head);
		tmp->next = new_node;
	}
}
