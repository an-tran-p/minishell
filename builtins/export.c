/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/13 20:52:51 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env(char **env)
{
	int		i;
	char	*tmp;
	int		sorted;

	sorted = 0;
	while (!sorted)
	{
		i = 0;
		sorted = 1;
		while (env[i] && env[i + 1])
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				sorted = 0;
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
			}
			i++;
		}
	}
}

int	export_print(char **env)
{
	char	**env_copy;
	int		i;
	char	*eq;

	env_copy = copy_env(env);
	if (!env_copy)
		return (1);
	sort_env(env_copy);
	i = 0;
	while (env_copy[i])
	{
		eq = ft_strchr(env_copy[i], '=');
		if (eq)
		{
			*eq = '\0';
			ft_printf("declare -x %s=\"%s\"\n", env_copy[i], eq + 1);
			*eq = '=';
		}
		else if (!eq)
			ft_printf("declare -x %s\n", env_copy[i]);
		i++;
	}
	ft_free_strarr(env_copy);
	return (0);
}

int	check_valid_id(char *entry, char c)
{
	int	i;

	if (ft_isalpha(entry[0]) == 0 && entry[0] != '_')
	{
		if (c == 'e')
			ft_printf("minishell: export: \'%s\': not a valid identifier\n",
				entry);
		return (1);
	}
	i = 1;
	while (entry[i] && entry[i] != '=')
	{
		if (ft_isalnum(entry[i]) == 0 && entry[i] != '_')
		{
			if (c == 'e')
				ft_printf("minishell: export: \'%s\': not a valid identifier\n",
					entry);
			return (1);
		}
		i++;
	}
	return (0);
}

char	**get_entry(char *argv)
{
	char	**entry;
	char	*eq;
	char	*key;

	eq = ft_strchr(argv, '=');
	if (eq)
	{
		entry = ft_split(argv, '=');
		if (!entry)
			return (NULL);
		key = ft_strjoin(entry[0], "=");
		if (!key)
			return (ft_free_strarr(entry), NULL);
		free(entry[0]);
		entry[0] = key;
	}
	else
	{
		entry = malloc(sizeof(char *) * 2);
		if (!entry)
			return (NULL);
		entry[0] = ft_strdup(argv);
		entry[1] = NULL;
	}
	return (entry);
}

int	ft_export(char **argv, char ***env)
{
	int		i;
	char	**entry;

	if (!argv[1])
		return (export_print(*env));
	else
	{
		i = 1;
		while (argv[i])
		{
			if (check_valid_id(argv[i], 'e') == 0)
			{
				entry = get_entry(argv[i]);
				if (!entry)
					return (1);
				ft_setenv(env, entry[0], entry[1]);
				ft_free_strarr(entry);
			}
			i++;
		}
	}
	return (0);
}
