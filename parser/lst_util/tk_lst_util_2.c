/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_lst_util_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:34:32 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/23 17:20:08 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tokenizer.h"

/*
void	tk_lstdelone(t_token **token)
{
	t_token	*tmp;

	if (!(*token))
		return ;
	tmp = *token;
	*token = (*token)->next;
	free(tmp->s);
	free(tmp);
}
*/

void	tk_lstclear(t_token **head)
{
	t_token	*tmp;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		free(tmp->s);
		free(tmp);
	}
}

/*
t_token	*tk_lstnth(t_token *head, size_t n)
{
	size_t	i;
	t_token	*tmp;

	tmp = head;
	i = 0;
	while (i < n && tmp)
	{
		tmp = tmp->next;
		i ++;
	}
	if (i != n)
		return (NULL);
	return (tmp);
}
*/

void	tk_appendlst(t_token **head, t_etype type, char *s)
{
	t_token	*new_node;

	new_node = tk_lstnew(type, s);
	if (!new_node)
		tk_lstclear(head);
	else
		tk_lstadd_back(head, new_node);
}

int	tk_appendfront(t_token **cur, char *s)
{
	t_token	*new_node;

	new_node = tk_lstnew(WORD, s);
	if (!new_node)
		return (-1);
	(*cur)->s = new_node->s;
	new_node->next = (*cur)->next;
	(*cur)->next = new_node;
	(*cur)->type = WORD;
	*cur = (*cur)->next;
	(*cur)->s = NULL;
	return (0);
}
