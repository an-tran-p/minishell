/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/26 11:15:38 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_key_to_remove(char ***env, char *key, size_t k_len)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, k_len) == 0)
		{
			if (ft_strlen((*env)[i]) == k_len || (ft_strlen((*env)[i]) > k_len
					&& (*env)[i][k_len] == '='))
			{
				ft_free_str(&(*env)[i]);
				while ((*env)[i + 1])
				{
					(*env)[i] = (*env)[i + 1];
					i++;
				}
				(*env)[i] = NULL;
				return (0);
			}
		}
		i++;
	}
	return (0);
}

int	remove_env_key(char ***env, char *key)
{
	size_t	k_len;

	if (!env || !(*env) || !key)
		return (0);
	k_len = ft_strlen(key);
	return (find_key_to_remove(env, key, k_len));
}

int	ft_unset(char **argv, char ***env)
{
	int	i;
	int	exit;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (check_valid_id(argv[i], 'u'))
			return (0);
		if (ft_strchr(argv[i], '='))
			return (0);
		else
			exit = remove_env_key(env, argv[i]);
		i++;
	}
	return (exit);
}
