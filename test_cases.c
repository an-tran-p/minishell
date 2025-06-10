/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/10 22:27:39 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*rd_append(t_token *head, t_token *new)
{
	t_token	*curr;

	if (!head)
		return (new);
	curr = head;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	return (head);
}

t_token	*create_rd(t_etype type, const char *s)
{
	t_token	*rd;

	rd = malloc(sizeof(t_token));
	if (!rd)
		return (NULL);
	rd->type = type;
	rd->s = strdup(s);
	rd->next = NULL;
	return (rd);
}

char	**dup_cmd(char *const *cmd)
{
	int		count;
	char	**copy;

	count = 0;
	while (cmd[count])
		count++;
	copy = malloc((count + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	for (int i = 0; i < count; i++)
	{
		copy[i] = strdup(cmd[i]);
		if (!copy[i])
		{
			for (int j = 0; j < i; j++)
				free(copy[j]);
			free(copy);
			return (NULL);
		}
	}
	copy[count] = NULL;
	return (copy);
}

static void	free_cmd_array(char **cmd)
{
	if (!cmd)
		return ;
	for (int i = 0; cmd[i]; i++)
		free(cmd[i]);
	free(cmd);
}

static void	free_steps_on_failure(t_step **steps, int count)
{
	for (int i = 0; i < count; i++)
	{
		if (steps[i])
		{
			free_cmd_array(steps[i]->cmd);
			free(steps[i]);
		}
	}
}

t_step	*test_1(void)
{
	t_step	*n[4];

	for (int i = 0; i < 4; i++)
		n[i] = malloc(sizeof(t_step));
	if (!n[0] || !n[1] || !n[2] || !n[3])
	{
		free_steps_on_failure(n, 4);
		return (NULL);
	}
	if (!(n[0]->cmd = dup_cmd((char *[]){"echo", "Test", NULL}))
		|| !(n[1]->cmd = dup_cmd((char *[]){"cat", "test.txt", NULL}))
		|| !(n[2]->cmd = dup_cmd((char *[]){"grep", "Te", NULL}))
		|| !(n[3]->cmd = dup_cmd((char *[]){"wc", "-l", NULL})))
	{
		free_steps_on_failure(n, 4);
		return (NULL);
	}
	n[0]->rd = create_rd(RD_APPEND, "test.txt");
	n[0]->hd_fd = 0;
	n[0]->pipe = 1;
	n[0]->next = n[1];
	n[1]->rd = NULL;
	n[1]->hd_fd = 0;
	n[1]->pipe = 1;
	n[1]->next = n[2];
	n[2]->rd = NULL;
	n[2]->hd_fd = 0;
	n[2]->pipe = 1;
	n[2]->next = n[3];
	n[3]->rd = NULL;
	n[3]->hd_fd = 0;
	n[3]->pipe = 0;
	n[3]->next = NULL;
	return (n[0]);
}

t_step	*test_2(void)
{
	t_step	*n;

	n = malloc(sizeof(t_step));
	if (!n)
		return (NULL);
	if (!(n->cmd = dup_cmd((char *[]){"echo", "Test", NULL})))
		return (free(n), NULL);
	n->rd = create_rd(RD_APPEND, "test.txt");
	n->hd_fd = 0;
	n->pipe = 0;
	n->next = NULL;
	return (n);
}

t_step	*test_3(void)
{
	t_step	*n;

	n = malloc(sizeof(t_step));
	if (!n)
		return (NULL);
	if (!(n->cmd = dup_cmd((char *[]){"export", NULL})))
		return (free(n), NULL);
	n->rd = NULL;
	n->hd_fd = 0;
	n->pipe = 0;
	n->next = NULL;
	return (n);
}

t_step	*test_4(void)
{
	t_step	*n[2];

	n[0] = malloc(sizeof(t_step));
	n[1] = malloc(sizeof(t_step));
	if (!n[0] || !n[1])
	{
		free(n[0]);
		free(n[1]);
		return (NULL);
	}
	if (!(n[0]->cmd = dup_cmd((char *[]){"export", NULL}))
		|| !(n[1]->cmd = dup_cmd((char *[]){"wc", "-l", NULL})))
	{
		free_steps_on_failure(n, 2);
		return (NULL);
	}
	n[0]->rd = NULL;
	n[0]->hd_fd = 0;
	n[0]->pipe = 1;
	n[0]->next = n[1];
	n[1]->rd = create_rd(RD_APPEND, "test.txt");
	n[1]->hd_fd = 0;
	n[1]->pipe = 0;
	n[1]->next = NULL;
	return (n[0]);
}

t_step	*test_5(void)
{
	t_step	*n[3];

	n[0] = malloc(sizeof(t_step));
	n[1] = malloc(sizeof(t_step));
	n[2] = malloc(sizeof(t_step));
	if (!n[0] || !n[1] || !n[2])
	{
		free_steps_on_failure(n, 3);
		return (NULL);
	}
	if (!(n[0]->cmd = dup_cmd((char *[]){"cat", NULL}))
		|| !(n[1]->cmd = dup_cmd((char *[]){"cat", NULL}))
		|| !(n[2]->cmd = dup_cmd((char *[]){"wc", "-l", NULL})))
	{
		free_steps_on_failure(n, 3);
		return (NULL);
	}
	n[0]->rd = NULL;
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_HDQUOTE, "eof"));
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_OUTFILE, "a"));
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_HDQUOTE, "abc"));
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_OUTFILE, "b"));
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_HDQUOTE, "def"));
	n[0]->rd = rd_append(n[0]->rd, create_rd(RD_OUTFILE, "c"));
	n[0]->hd_fd = 0;
	n[0]->pipe = 1;
	n[0]->next = n[1];
	n[1]->rd = rd_append(NULL, create_rd(RD_HDQUOTE, "an"));
	n[1]->hd_fd = 0;
	n[1]->pipe = 1;
	n[1]->next = n[2];
	n[2]->rd = create_rd(RD_INFILE, "test.txt");
	n[2]->hd_fd = 0;
	n[2]->pipe = 0;
	n[2]->next = NULL;
	return (n[0]);
}

void	free_rd(t_token *rd)
{
	t_token	*tmp;

	while (rd)
	{
		tmp = rd->next;
		free(rd->s);
		free(rd);
		rd = tmp;
	}
}

void	ft_free_step(t_step *step)
{
	t_step	*tmp;

	while (step)
	{
		tmp = step->next;
		if (step->rd)
			free_rd(step->rd);
		if (step->cmd)
			ft_free_strarr(step->cmd);
		free(step);
		step = tmp;
	}
}
