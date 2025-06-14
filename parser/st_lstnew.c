/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/14 02:36:56 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static t_token	*new_node_rd(t_token **head, t_token *tmp, t_token *pre,
		t_step *new_node)
{
	if (!pre)
		*head = tmp->next;
	else
		pre->next = tmp->next;
	if (tmp->type == DEL)
	{
		free(tmp->s);
		tmp->s = NULL;
		free(tmp);
	}
	else
	{
		tmp->next = NULL;
		tk_lstadd_back(&new_node->rd, tmp);
	}
	if (!pre)
		tmp = *head;
	else
		tmp = pre->next;
	return (tmp);
}

static int	cmd_count(t_token **head, t_step *new_node)
{
	int		w_count;
	t_token	*tmp;
	t_token	*pre;

	w_count = 0;
	pre = NULL;
	tmp = *head;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD)
		{
			w_count++;
			pre = tmp;
			tmp = tmp->next;
		}
		else
		{
			tmp = new_node_rd(head, tmp, pre, new_node);
		}
	}
	return (w_count);
}

static void	new_node_cmd(t_token **head, t_step *new_node, int w_count)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (i < w_count)
	{
		tmp = *head;
		*head = tmp->next;
		new_node->cmd[i] = tmp->s;
		free(tmp);
		i++;
	}
	new_node->cmd[i] = NULL;
}

static void	chk_pipe(t_token **head, t_step *new_node)
{
	t_token	*tmp;

	if ((*head) && (*head)->type == PIPE)
	{
		tmp = *head;
		new_node->pipe = 1;
		*head = tmp->next;
		free(tmp->s);
		free(tmp);
	}
}

t_step	*st_lstnew(t_token **head)
{
	int		w_count;
	t_step	*new_node;

	new_node = (t_step *)ft_calloc(1, sizeof(t_step));
	if (!new_node)
		return (NULL);
	w_count = cmd_count(head, new_node);
	if (w_count)
	{
		new_node->cmd = (char **)ft_calloc(w_count + 1, sizeof(char *));
		if (!new_node->cmd)
		{
			tk_lstclear(&new_node->rd);
			free(new_node);
			return (NULL);
		}
		new_node_cmd(head, new_node, w_count);
	}
	chk_pipe(head, new_node);
	return (new_node);
}
