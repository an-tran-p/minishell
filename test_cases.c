/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/04 23:07:24 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_rd(t_etype type, const char *s);
char	**dup_cmd(char *const *cmd);

// exmaple: echo Test >> test.txt | cat test.txt | grep Te | wc-l
t_step	*test_1(void)
{
	t_step	*n1;
	t_step	*n2;
	t_step	*n3;
	t_step	*n4;

	n1 = malloc(sizeof(t_step));
	n2 = malloc(sizeof(t_step));
	n3 = malloc(sizeof(t_step));
	n4 = malloc(sizeof(t_step));
	if (!n1 || !n2 || !n3 || !n4)
	{
		free(n1);
		free(n2);
		free(n3);
		free(n4);
		return (NULL);
	}
	// Duplicate cmd arrays properly
	if (!(n1->cmd = dup_cmd((char *[]){"echo", "Test", NULL}))
		|| !(n2->cmd = dup_cmd((char *[]){"cat", "test.txt", NULL}))
		|| !(n3->cmd = dup_cmd((char *[]){"grep", "Te", NULL}))
		|| !(n4->cmd = dup_cmd((char *[]){"wc", "-l", NULL})))
	{
		for (t_step *n = n1; n != NULL; n = n->next)
		{
			if (n->cmd)
			{
				for (int i = 0; n->cmd[i]; i++)
					free(n->cmd[i]);
				free(n->cmd);
			}
		}
		free(n1);
		free(n2);
		free(n3);
		free(n4);
		return (NULL);
	}
	// Setup redirections and pipes
	n1->rd = create_rd(RD_APPEND, "test.txt");
	n1->pipe = 1;
	n1->next = n2;
	n2->rd = NULL;
	n2->pipe = 1;
	n2->next = n3;
	n3->rd = NULL;
	n3->pipe = 1;
	n3->next = n4;
	n4->rd = NULL;
	n4->pipe = 0;
	n4->next = NULL;
	return (n1);
}
// exmaple: echo Test >> test.txt
t_step	*test_2(void)
{
	t_step	*n1;

	n1 = malloc(sizeof(t_step));
	if (!n1)
		return (NULL);
	// Duplicate cmd arrays properly
	if (!(n1->cmd = dup_cmd((char *[]){"echo", "Test", NULL})))
		return (NULL);
	// Setup redirections and pipes
	n1->rd = create_rd(RD_APPEND, "test.txt");
	n1->pipe = 0;
	n1->next = NULL;
	return (n1);
}

// exmaple: export
t_step	*test_3(void)
{
	t_step	*n1;

	n1 = malloc(sizeof(t_step));
	if (!n1)
		return (NULL);
	// Duplicate cmd arrays properly
	if (!(n1->cmd = dup_cmd((char *[]){"export", NULL})))
		return (NULL);
	// Setup redirections and pipes
	n1->rd = NULL;
	n1->pipe = 0;
	n1->next = NULL;
	return (n1);
}

// exmaple: export > test.txt | <test.txt grep "declare" | wc -l >> test.txt
t_step	*test_4(void)
{
	t_step	*n1;
	t_step	*n2;
	t_step	*n3;

	n1 = malloc(sizeof(t_step));
	n2 = malloc(sizeof(t_step));
	n3 = malloc(sizeof(t_step));
	if (!n1 || !n2 || !n3)
	{
		free(n1);
		free(n2);
		free(n3);
		return (NULL);
	}
	// Duplicate cmd arrays properly
	if (!(n1->cmd = dup_cmd((char *[]){"export", NULL}))
		|| !(n2->cmd = dup_cmd((char *[]){"grep", "declare", NULL}))
		|| !(n3->cmd = dup_cmd((char *[]){"wc", "-l", NULL})))
	{
		for (t_step *n = n1; n != NULL; n = n->next)
		{
			if (n->cmd)
			{
				for (int i = 0; n->cmd[i]; i++)
					free(n->cmd[i]);
				free(n->cmd);
			}
		}
		free(n1);
		free(n2);
		free(n3);
		return (NULL);
	}
	// Setup redirections and pipes
	n1->rd = create_rd(RD_OUTFILE, "test.txt");
	n1->pipe = 1;
	n1->next = n2;
	n2->rd = create_rd(RD_INFILE, "test.txt");
	n2->pipe = 1;
	n2->next = n3;
	n3->rd = create_rd(RD_APPEND, "test.txt");
	n3->pipe = 0;
	n3->next = NULL;
	return (n1);
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
			// Free previously allocated strings on failure
			for (int j = 0; j < i; j++)
				free(copy[j]);
			free(copy);
			return (NULL);
		}
	}
	copy[count] = NULL;
	return (copy);
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
