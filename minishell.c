/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/06 17:41:10 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(char *argv)
{
	if (ft_strncmp(argv, "echo", 5) == 0 || ft_strncmp(argv, "cd", 3) == 0
		|| ft_strncmp(argv, "pwd", 4) == 0 || ft_strncmp(argv, "export", 7) == 0
		|| ft_strncmp(argv, "unset", 6) == 0 || ft_strncmp(argv, "env", 4) == 0
		|| ft_strncmp(argv, "exit", 5) == 0)
		return (0);
	return (1);
}

int	ft_cd(char **argv, char **env)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	old_pwd[PATH_MAX];

	if (!argv[1])
		path = ft_getenv(env, "HOME");
	else if (ft_strncmp(argv[1], "-", 2) == 0)
		path = ft_getenv(env, "OLDPWD");
	else if (ft_strncmp(argv[1], "..", 3) == 0)
	{
		getcwd(cwd, PATH_MAX);
		*(ft_strrchr(cwd, '/')) = '\0';
		path = cwd;
	}
	else
		path = argv[1];
	getcwd(old_pwd, PATH_MAX);
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s", argv[1]),
		perror(": ");
		return (1);
	}
	return (0);
}

int	execute_builtin(char **argv, char **env)
{
	int	exit;

	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exit = ft_echo(argv);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exit = ft_pwd();
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		exit = ft_cd(argv, env);
	return (exit);
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;

	if (argc < 0)
		return (-1);
	env = copy_envp(envp);
	if (!env)
		return (-1);
	if (is_builtins(argv[1]) == 0)
		execute_builtin(&argv[1], env);
	ft_free_strarr(env);
	return (0);
}
