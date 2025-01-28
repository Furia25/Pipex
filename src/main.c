/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/01/28 14:21:19 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

int	open_file(char *path, int mode)
{
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
	char	heredoc[1024];
	size_t	limiter_length;
	size_t	readed;
	int		file;
	
	if (access("./", R_OK | W_OK) != 0)
		return (perror("Access"), 0);
	file = open_file(TEMP_PATH, 2);
	if (!file)
		return (perror("Open"), 0);
	limiter_length = ft_strlen(limiter);
	while (1)
	{
		ft_putstr_fd("\e[32;47mheredoc >\e[0m ", 2);
		readed = read(STDIN_FILENO, heredoc, 1023);
		heredoc[readed] = '\0';
		if (ft_strncmp(heredoc, limiter, limiter_length) == 0)
			break ;
		ft_putstr_fd(heredoc, file);
	}
	return (close(file), 1);
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
	paths = ft_split(envp[index] + 5, ':');
	if (!paths)
		return (NULL);
	index = 0;
	while (paths[index])
	{
		path = ft_strjoin(paths[index], "/");
		path = ft_nstrjoin(path, command);
		if (!path)
			break;
		if (access(path, F_OK | X_OK) == 0)
			return (free_chartab(paths), path);
		free(path);
		index++;
	}
	free_chartab(paths);
	return (NULL);
}

int	parse_commands(char **cmds, char **names, char **envp)
{
	char 	*temp;
	size_t	index;

	index = 0;
	while (names[index] && names[index + 1])
	{
		cmds[index] = find_command(names[index], envp);
		if (!cmds[index])
		{
			temp = ft_strjoin("Command not found: ", names[index]);
			if (temp)
				ft_putstr_fd(temp, 2);
			free(temp);
			return (0);
		}
		index ++;
	}
	cmds[index] = NULL;
	return (1);
}

static void	free_data(char **cmds, int (*pipes)[2])
{
	free_chartab(cmds);
	free(pipes);
}

void	close_pipes(int (*pipes)[2], size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		close(pipes[index][0]);
		close(pipes[index][1]);
		index++;
	}
	return ;
}

void	close_pipes_except(int (*pipes)[2], size_t i, size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		if (index == i)
		{
			index++;
			continue;
		}
		close(pipes[index][0]);
		close(pipes[index][1]);
		index++;
	}
	return ;
}

int	create_pipes(int (*pipes)[2], size_t count)
{
	size_t	index;

	index = 0;
	while (index < count)
	{
		if (pipe(pipes[index]) == -1)
		{
			close_pipes(pipes, index);
			return (perror("Pipes"), 0);
		}
		index++;
	}
	return (1);
}

int	pipex(char **cmds, int (*pipes)[2], size_t count, char **envp)
{
	size_t	index;
	pid_t	pid;

	index = 0;
	while(cmds[index])
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), close_pipes(pipes, count), 0);
		if (pid == 0)
		{
			close_pipes_except(pipes, index, count);
			
			execve(cmds[index], "", envp);
			perror("Execve");
			exit(EXIT_FAILURE);
		}
		index++;
	}
	close_pipes(pipes, count);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	size_t	index;
	int		here_doc;
	int		(*pipes)[2];
	char	**cmds;

	if (argc < 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	index = 2;
	here_doc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	if (here_doc)
		index = 3;
	cmds = ft_calloc(argc - index, sizeof(char *));
	if (!cmds)
		return (perror("malloc"), EXIT_FAILURE);
	if (!parse_commands(cmds, argv + index, envp))
		return (free_chartab(cmds), EXIT_FAILURE);
	pipes = ft_calloc(argc - index, sizeof(int[2]));
	if (!pipes)
		return (free_data(cmds, pipes), perror("Malloc"), EXIT_FAILURE);
	if (!create_pipes(pipes, argc - index))
		return (free_data(cmds, pipes), perror("Pipes"), EXIT_FAILURE);
	pipex(cmds, pipes, argc - index);
	unlink(TEMP_PATH);
    return (free_data(cmds, pipes), EXIT_SUCCESS);
}
