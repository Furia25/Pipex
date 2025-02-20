/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:33:49 by vdurand           #+#    #+#             */
/*   Updated: 2025/02/20 14:36:35 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	open_file(char *path, int mode, int *index, char *file);

int	pipex(int argc, char *argv[], char *envp[], char *name)
{
	int		last_fd;
	int		infile_fd;
	int		outfile_fd;
	int		index;

	index = 2;
	infile_fd = open_file(argv[1], 0, &index, name);
	if (infile_fd == -1)
		return (perror("File 1"), exit(EXIT_FAILURE), 1);
	outfile_fd = open_file(argv[argc - 1], 2, &index, name);
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

int	read_heredoc(char *limiter, char *file_name)
{
	char	*heredoc;
	size_t	limiter_length;
	int		file;

	if (access("./", R_OK | W_OK) != 0)
		return (perror("Access"), 0);
	file = open_file(file_name, 2, 0, NULL);
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

static int	open_file(char *path, int mode, int *index, char *file)
{
	if (file)
	{
		*index = 3;
		if (mode != 0)
			mode = 1;
		else
			path = file;
	}
	if (mode == 0)
		return (open(path, O_RDONLY));
	if (mode == 1)
		return (open(path, O_RDWR | O_APPEND | O_CREAT, 0777));
	if (mode == 2)
		return (open(path, O_RDWR | O_TRUNC | O_CREAT, 0777));
	return (-1);
}
