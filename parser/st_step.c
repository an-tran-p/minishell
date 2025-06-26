/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st_step.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/26 11:11:53 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	st_lstadd_back(t_step **lst, t_step *new_node)
{
	t_step	*tmp;

	tmp = *lst;
	if (!tmp)
		*lst = new_node;
	else if (new_node)
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	st_lstclear(t_step **lst)
{
	int		i;
	t_step	*tmp;

	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		tk_lstclear(&tmp->rd);
		i = 0;
		while (tmp->cmd && tmp->cmd[i])
		{
			free(tmp->cmd[i]);
			i++;
		}
		free(tmp->cmd);
		free(tmp);
	}
}

void	st_step(t_token **head, t_step **lst)
{
	t_step	*new_node;

	while (*head)
	{
		new_node = st_lstnew(head);
		if (!new_node)
		{
			tk_lstclear(head);
			st_lstclear(lst);
			ft_putstr_fd("Memory alllocation failed.\n", 2);
			exit(EXIT_FAILURE);
		}
		else
			st_lstadd_back(lst, new_node);
	}
}
