/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/01/29 17:00:13 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

int	open_file(char *path, int mode, int index)
{
	if (index == 3)
		path = TEMP_PATH;
	if (index == 3 && mode != 0)
		mode = 1;
	if (mode == 0)
		return (open(path, O_RDONLY));
	if (mode == 1)
		return (open(path, O_RDWR | O_APPEND | O_CREAT, 0777));
	if (mode == 2)
		return (open(path, O_RDWR | O_TRUNC | O_CREAT, 0777));
	return (-1);
}

char	*ft_nstrjoin(char *freable, const char *s2)
{
	char	*result;

	if (!freable)
		return (NULL);
	result = ft_strjoin(freable, s2);
	free(freable);
	return (result);
}

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
			break;
		if (ft_strncmp(heredoc, limiter, limiter_length) == 0)
			break ;
		ft_putstr_fd(heredoc, file);
	}
	free(heredoc);
	close(file);
	return (1);
}

void	free_chartab(char **tab)
{
	size_t	index;

	index = 0;
	while (tab[index])
	{
		free(tab[index]);
		index++;
	}
	free(tab);
}

char	*find_command(char *command, char **envp)
{
	size_t	index;
	char	**paths;
	char	*path;

	index = 0;
	while (!ft_strncmp(envp[index], "PATH", 4) == 0)
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
			break;
		if (access(path, F_OK | X_OK) == 0)
			return (free_chartab(paths), path);
		free(path);
	}
	free_chartab(paths);
	return (NULL);
}

void	cmd_execute(char *cmd, char **envp)
{
	char	**temp;
	char	*error_temp;
	char	*result;

	temp = ft_split(cmd, ' ');
	if (!temp)
		return ;
	result = find_command(temp[0], envp);
	if (!result)
	{
		error_temp = ft_strjoin(temp[0], "command not found");
		if(error_temp)
			ft_putstr_fd(error_temp, 2);
		free(error_temp);
		free(temp);
		return ;
	}
	execve(result, temp, envp);
	perror("Execve");
	free_chartab(temp);
	free(result);
	exit(EXIT_FAILURE);
}

int	pipe_and_process(char *cmd, char **envp, int *lastfd, int last)
{
	int		ppipe[2];
	pid_t	pid;
	
	if(pipe(ppipe) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		dup2(*lastfd, STDIN_FILENO);
		close(*lastfd);
		close(ppipe[0]);
		if (!last)
			dup2(ppipe[1], STDOUT_FILENO);
		close(ppipe[1]);
		cmd_execute(cmd, envp);
	}
	close(ppipe[1]);
	close(*lastfd);
	*lastfd = ppipe[0];
	return (1);
}

void	pipex(int argc, char *argv[], char *envp[], int index)
{
	int		last_fd;
	int		infile_fd;
	int		outfile_fd;

	infile_fd = open_file(argv[1], 0, index);
	if (infile_fd == -1)
		return(perror("File"), exit(EXIT_FAILURE));
	outfile_fd = open_file(argv[argc - 1], 2, 0);
	if (outfile_fd == -1)
		return(close(infile_fd), perror("File"), exit(EXIT_FAILURE));
	last_fd = infile_fd;
	while (index < argc - 2)
		pipe_and_process(argv[index++], envp, &last_fd, 0);
	dup2(outfile_fd, STDOUT_FILENO);
	pipe_and_process(argv[index], envp, &last_fd, 1);
}

int	main(int argc, char **argv, char **envp)
{
	int		index;

	if (argc < 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	index = 2;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			return (ft_putstr_fd(ERROR_HEREDOC_USAGE, 2), EXIT_FAILURE);
		if (!read_heredoc(argv[2]))
			exit(EXIT_FAILURE);
		index = 3;
	}
	pipex(argc, argv, envp, index);
	while (wait(NULL) > 0);
	unlink(TEMP_PATH);
    return (EXIT_SUCCESS);
}
