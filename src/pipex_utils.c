/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: val <val@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:58:21 by val               #+#    #+#             */
/*   Updated: 2025/02/05 17:50:35 by val              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
