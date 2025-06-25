/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:56:42 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 12:41:50 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**malloc_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	return (env);
}

char	**copy_env(char **envp)
{
	int		i;
	int		j;
	char	**env;

	env = malloc_env(envp);
	if (!env)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		env[j] = ft_strdup(envp[i]);
		if (!env[j])
		{
			ft_free_strarr(env);
			return (NULL);
		}
		i++;
		j++;
	}
	env[j] = NULL;
	return (env);
}
