/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/19 17:53:07 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_not_n(char *s)
{
	int	i;

	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	ft_echo(char **argv)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		if (!check_not_n(argv[1]))
		{
			n = 1;
			i = 1;
		}
	}
	while (argv[++i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
	}
	if (n == 0)
		ft_printf("\n");
	return (0);
}
