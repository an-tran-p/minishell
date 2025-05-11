/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/05/11 21:30:24 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_valid_id(char *entry)
{
	int	i;

	if (ft_isalpha(entry[0]) == 0 && entry[0] != '_')
	{
		ft_printf("minishell: export: \'%s\': not a valid identifier", entry);
		return (1);
	}
	i = 1;
	while (entry[i] && entry[i] != '=')
	{
		if (ft_isalnum(entry[i] == 0 && entry[i] != '_'))
		{
			ft_printf("minishell: export: \'%s\': not a valid identifier",
				entry);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_export(char **argv, char **env)
{
	int i;
	char *eq;
	char *key;

	if (!argv[1])
		return (export_print(env));
	else
	{
		i = 1;
		while (argv[i])
		{
			if (check_valid_id(argv[i]) == 0)
			{
				eq = ft_strchr(argv[i], '=');
				if (eq)
				{
					ft_setenv();
				}
			}
		}
	}
	return (0);
}