/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:58:21 by val               #+#    #+#             */
/*   Updated: 2025/02/13 16:31:52 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	read_heredoc(char *limiter)
{
	char	*heredoc;
	size_t	limiter_length;
	int		file;

	if (access("./", R_OK | W_OK) != 0)
		return (perror("Access"), 0);
	file = open_file(TEMP_PATH, 2, 0);
	if (file == -1)
		return (perror("Open"), 0);
	limiter_length = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("\e[32;47mheredoc >\e[0m ", 2);
		heredoc = get_next_line(0);
		if (!heredoc)
			break ;
		if (ft_strncmp(heredoc, limiter, limiter_length) == 0)
			break ;
		ft_putstr_fd(heredoc, file);
		free(heredoc);
	}
	free(heredoc);
	close(file);
	return (1);
}

char	*find_command_path(char *command, char **envp)
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

int	cmd_execute(char *cmd, char **envp)
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
			return (perror("Dup"), close(*lfd), close(pip[0]), close(pip[1]), 0);
		close(*lfd);
		close(pip[0]);
		if (!last)
			if (dup2(pip[1], STDOUT_FILENO) == -1)
				return (perror("Dup"), close(pip[1]), 0);
		close(pip[1]);
		if (!cmd_execute(cmd, envp))
			return (exit(127), 0);
	}
	close(pip[1]);
	close(*lfd);
	*lfd = pip[0];
	return (1);
}

int	pipex(int argc, char *argv[], char *envp[], int index)
{
	int		last_fd;
	int		infile_fd;
	int		outfile_fd;

	infile_fd = open_file(argv[1], 0, index);
	if (infile_fd == -1)
		return (perror("File 1"), exit(EXIT_FAILURE), 1);
	outfile_fd = open_file(argv[argc - 1], 2, 0);
	if (outfile_fd == -1)
		return (close(infile_fd), perror("File 2"), exit(EXIT_FAILURE), 1);
	last_fd = infile_fd;
	while (index < argc - 2)
		if (!pipe_and_process(argv[index++], envp, &last_fd, 0))
			return (close(outfile_fd), 0);
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		return (perror("Dup"), close(last_fd), close(outfile_fd), 0);
	if (!pipe_and_process(argv[index], envp, &last_fd, 1))
		return (close(outfile_fd), 0);
	else
		close(last_fd);
	close(outfile_fd);
	return (1);
}
