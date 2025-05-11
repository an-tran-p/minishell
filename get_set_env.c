/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:56:42 by atran             #+#    #+#             */
/*   Updated: 2025/05/11 19:31:55 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_setenv(char **env, char *key, char *n_value)
{
	char	*new_entry;
	int		i;

	new_entry = ft_strjoin(key, n_value);
	if (!new_entry)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			ft_free_str(&env[i]);
			env[i] = new_entry;
			return (0);
		}
		i++;
	}
	env[i] = new_entry;
	env[i + 1] = NULL;
	return (0);
}

char	*ft_getenv(char **env, char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
		{
			if (env[i][len + 1])
				return (env[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			ft_free_strarr(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}
