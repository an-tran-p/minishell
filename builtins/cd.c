/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 13:06:06 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_err(void)
{
	ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
	ft_putstr_fd("cannot access parent directories: ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
}

char	*get_cd_path(char **argv, char **env)
{
	char	*path;

	if (argv[1] && argv[2])
		return (ft_put_err("too many argument", "cd", NULL), NULL);
	else if (!argv[1] || (argv[1] && ft_strncmp(argv[1], "~", 2) == 0))
	{
		path = ft_getenv(env, "HOME");
		if (!path)
		{
			ft_put_err("HOME not set", "cd", NULL);
			return (NULL);
		}
	}
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = ft_getenv(env, "OLDPWD");
		if (!path)
		{
			ft_put_err("OLDPWD not set", "cd", NULL);
			return (NULL);
		}
	}
	else
		path = argv[1];
	return (path);
}

int	ft_cd(char **argv, char ***env)
{
	char	*path;
	char	old[PATH_MAX];
	char	new[PATH_MAX];
	char	*old_pwd;

	path = get_cd_path(argv, *env);
	if (!path)
		return (1);
	old_pwd = getcwd(old, PATH_MAX);
	if (chdir(path) != 0)
	{
		ft_put_err(strerror(errno), "cd", path);
		return (1);
	}
	path = getcwd(new, PATH_MAX);
	if (!path && errno == ENOENT)
	{
		cd_err();
		return (1);
	}
	ft_setenv(env, "OLDPWD=", old_pwd);
	ft_setenv(env, "PWD=", path);
	return (0);
}
