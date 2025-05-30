/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:56:42 by atran             #+#    #+#             */
/*   Updated: 2025/05/31 23:16:54 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_new_entry(char *k, char *n_value)
{
	char	*new_entry;

	if (!n_value)
		new_entry = ft_strdup(k);
	else
		new_entry = ft_strjoin(k, n_value);
	if (!new_entry)
		return (NULL);
	return (new_entry);
}

int	ft_setenv(char ***env, char *k, char *n_value)
{
	char	*new_entry;
	int		i;

	new_entry = get_new_entry(k, n_value);
	if (!new_entry)
		return (1);
	i = -1;
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], k, ft_strlen(k)) == 0 && ft_strchr(k, '='))
		{
			ft_free_str(&(*env)[i]);
			(*env)[i] = new_entry;
			return (0);
		}
		else if (ft_strncmp((*env)[i], k, ft_strchr((*env)[i], '=')
				- (*env)[i]) == 0 && !ft_strchr(k, '='))
			return (0);
	}
	*env = realloc_env(*env, 1);
	if (!(*env))
		return (ft_free_str(&new_entry), 1);
	(*env)[i] = new_entry;
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

char	**realloc_env(char **env, int add)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1 + add));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = NULL;
	new_env[i + 1] = NULL;
	free(env);
	env = new_env;
	return (env);
}
