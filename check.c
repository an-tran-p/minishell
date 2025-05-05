/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:03:58 by atran             #+#    #+#             */
/*   Updated: 2025/05/05 17:04:31 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_space(char *argv)
{
	int		flag1;
	int		flag2;
	char	*str;

	flag1 = 0;
	flag2 = 0;
	str = argv;
	while (*str)
	{
		if (*str == '\'')
			flag1++;
		if (*str == '\"')
			flag2++;
		if (*str == ' ' && (flag1 % 2 != 0 || flag2 % 2 != 0))
			return (str);
		str++;
	}
	return (NULL);
}

char	replace_chr(char *argv)
{
	char	c;
	int		i;
	int		flag;

	c = 33;
	flag = 0;
	while (c <= 126)
	{
		i = 0;
		while (argv[i])
		{
			if (argv[i] == c)
				flag = 1;
			i++;
		}
		if (flag == 0 && c != '0')
			return (c);
		c++;
	}
	return ('0');
}

char	replace_space_in_cmd(char *argv)
{
	char	rp;
	char	*space;

	rp = replace_chr(argv);
	if (rp == '0')
		return (rp);
	while (check_space(argv))
	{
		space = check_space(argv);
		*space = rp;
	}
	return (rp);
}

void	put_back_space(char **cmd, char c)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == c)
				cmd[i][j] = ' ';
			j++;
		}
		i++;
	}
}
