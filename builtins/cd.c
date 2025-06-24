/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 21:18:55 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cd(char **argv, char **env)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	cur_pwd[PATH_MAX];

	if (argv[2])
	{
		ft_put_err("too many argument", "cd", NULL);
		return (1);
	}
	else if (!argv[1] || (argv[1] && ft_strncmp(argv[1], "~", 2) == 0))
	{
		path = ft_getenv(env, "HOME");
		if (!path)
		{
			ft_put_err("HOME not set", "cd", NULL);
			return (1);
		}
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = ft_getenv(env, "OLDPWD");
		if (!path)
		{
			ft_put_err("OLDPWD not set", "cd", NULL);
			return (1);
		}
	}
	else
		path = argv[1];
	getcwd(old_pwd, PATH_MAX);
	if (chdir(path) != 0)
	{
		ft_put_err(strerror(errno), "cd", path);
		return (1);
	}
	ft_setenv(&env, "OLDPWD=", old_pwd);
	getcwd(cur_pwd, PATH_MAX);
	ft_setenv(&env, "PWD=", cur_pwd);
	return (0);
}
