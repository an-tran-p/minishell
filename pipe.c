/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:02:26 by atran             #+#    #+#             */
/*   Updated: 2025/05/05 18:05:30 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	char	rp;

	rp = replace_space_in_cmd(argv);
	cmd = ft_split(argv, ' ');
	if (rp != '0')
		put_back_space(cmd, rp);
	path = find_path(cmd[0], envp);
	if (!path)
	{
		ft_put_err("command not found: ", cmd[0]);
		ft_free_strarr(cmd);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_free_strarr(cmd);
		free(path);
		perror("Execution fails\n");
		exit(127);
	}
	ft_free_strarr(cmd);
	free(path);
	exit(0);
}

void	execute_child1_process(int *fds, char *file1, char *cmd1, char **envp)
{
	int	fd1;

	close(fds[0]);
	fd1 = open(file1, O_RDONLY);
	if (fd1 == -1)
	{
		ft_printf("%s: %s\n", strerror(errno), file1);
		exit(1);
	}
	dup2(fd1, STDIN_FILENO);
	close(fd1);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);
	execute(cmd1, envp);
}

void	execute_child2_process(int *fds, char *file2, char *cmd2, char **envp)
{
	int	fd2;

	close(fds[1]);
	fd2 = open(file2, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd2 == -1)
	{
		ft_printf("%s: %s\n", strerror(errno), file2);
		exit(1);
	}
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
	execute(cmd2, envp);
}

int	create_processes(int *fds, char **argv, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Failed creating process\n");
		return (1);
	}
	if (pid1 == 0)
		execute_child1_process(fds, argv[1], argv[2], envp);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Failed creating process\n");
		return (1);
	}
	if (pid2 == 0)
		execute_child2_process(fds, argv[4], argv[3], envp);
	close(fds[0]);
	close(fds[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}

/* int	main(int argc, char **argv, char **envp)
{
	int		fds[2];
	int		status;

	if (argc != 5)
	{
		ft_printf("Error: 4 arguments needed <file1> <cmd1> <cmd2> <file2>\n");
		exit(1);
	}
	if (pipe(fds) == -1)
	{
		perror("Failed creating pipe\n");
		exit(1);
	}

	status = create_processes(fds, argv, envp);
	return (status);
} */
