/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_entry_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:56:42 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 19:06:08 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_new_entry(size_t *len, char *k, char *n_value)
{
	char	*new_entry;

	if (ft_strchr(k, '='))
		*len = ft_strlen(k) - 1;
	else if (!ft_strchr(k, '='))
		*len = ft_strlen(k);
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
	size_t	len;

	new_entry = get_new_entry(&len, k, n_value);
	if (!new_entry)
		return (1);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], k, len) == 0)
		{
			if (ft_strlen((*env)[i]) == len || (ft_strlen((*env)[i]) > len
					&& (*env)[i][len] == '='))
			{
				if (strchr(k, '='))
				{
					ft_free_str(&(*env)[i]);
					(*env)[i] = new_entry;
					return (0);
				}
				else
					return (0);
			}
		}
		else if (ft_strncmp((*env)[i], k, ft_strchr((*env)[i], '=')
				- (*env)[i]) == 0 && !ft_strchr(k, '='))
			return (0);
		i++;
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
	return (new_env);
}
