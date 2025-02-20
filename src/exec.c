/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:38:31 by vdurand           #+#    #+#             */
/*   Updated: 2025/02/20 14:43:38 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*find_command_path(char *command, char **envp);
static int	cmd_execute(char *cmd, char **envp);

int	pipe_and_process(char *cmd, char **envp, int *lfd, int last)
{
	int		pip[2];
	pid_t	pid;

	if (pipe(pip) == -1)
		return (perror("Pipe"), close(*lfd), 0);
	pid = fork();
	if (pid == -1)
		return (perror("Fork"), close(*lfd), 0);
	if (pid == 0)
	{
		if (dup2(*lfd, STDIN_FILENO) == -1)
			return (perror("D2"), close(*lfd), close(pip[0]), close(pip[1]), 0);
		close(*lfd);
		close(pip[0]);
		if (!last)
			if (dup2(pip[1], STDOUT_FILENO) == -1)
				return (perror("Dup2"), close(pip[1]), 0);
		close(pip[1]);
		if (!cmd_execute(cmd, envp))
			return (exit(127), 0);
	}
	close(pip[1]);
	close(*lfd);
	*lfd = pip[0];
	return (1);
}

static char	*find_command_path(char *command, char **envp)
{
	size_t	index;
	char	**paths;
	char	*path;

	index = 0;
	while (envp[index] && !ft_strncmp(envp[index], "PATH", 4) == 0)
		index++;
	if (!envp[index])
		return (NULL);
	paths = ft_split(envp[index] + 5, ':');
	if (!paths)
		return (NULL);
	index = 0;
	while (paths[index])
	{
		path = ft_strjoin(paths[index++], "/");
		path = ft_nstrjoin(path, command);
		if (!path)
			break ;
		if (access(path, F_OK | X_OK) == 0)
			return (free_chartab(paths), path);
		free(path);
	}
	free_chartab(paths);
	return (NULL);
}

static int	cmd_execute(char *cmd, char **envp)
{
	char	**temp;
	char	*error_temp;
	char	*result;

	temp = smart_split(cmd, ' ');
	if (!temp)
		return (perror("Malloc"), 0);
	result = NULL;
	if (access(temp[0], F_OK | X_OK) == 0)
		result = temp[0];
	else if (envp[0])
		result = find_command_path(temp[0], envp);
	if (!result)
	{
		ft_putstr_fd("\033[41m\"", 2);
		error_temp = ft_strjoin(temp[0], "\" command not found!");
		if (error_temp)
			ft_putstr_fd(error_temp, 2);
		ft_putstr_fd("\033[0m\n", 2);
		return (free(error_temp), free_chartab(temp), 0);
	}
	execve(result, temp, envp);
	perror("Execve");
	free_chartab(temp);
	return (free(result), exit(EXIT_FAILURE), 0);
}
