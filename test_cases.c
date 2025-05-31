/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/31 23:09:14 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exmaple: echo Test >> test.txt | cat Test.tct | grep Te | wc-l
t_rd	*create_rd(t_etype type, const char *s)
{
	t_rd	*rd;

	rd = malloc(sizeof(t_rd));
	if (!rd)
		return (NULL);
	rd->type = type;
	rd->s = strdup(s);
	rd->next = NULL;
	return (rd);
}

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
		return (NULL);
	// Node 1: echo Test >> test.txt
	n1->cmd = (char *[]){"echo", "Test", NULL};
	n1->rd = create_rd(RD_APPEND, "test.txt");
	n1->pipe = 1;
	n1->next = n2;
	// Node 2: cat test.txt
	n2->cmd = (char *[]){"cat", "test.txt", NULL};
	n2->rd = NULL; // cat reads from previous pipe, not file
	n2->pipe = 1;
	n2->next = n3;
	// Node 3: grep Te
	n3->cmd = (char *[]){"grep", "Te", NULL};
	n3->rd = NULL;
	n3->pipe = 1;
	n3->next = n4;
	// Node 4: wc -l
	n4->cmd = (char *[]){"wc", "-l", NULL};
	n4->rd = NULL;
	n4->pipe = 0;
	n4->next = NULL;
	return (n1);
}
