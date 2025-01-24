/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/01/24 15:39:03 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

char	*ft_fstrjoin(char *s1, const char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

int	error(void)
{
	perror("Failed");
	return (0);
}

int	read_heredoc(char *LIMITER)
{
	char	heredoc[1024];
	char	*search;
	size_t	readed;
	int		file;
	
	if (access("./", R_OK | W_OK) != 0)
		return (error());
	file = open_file(TEMP_PATH, 2);
	if (!file)
		return (error());
	while (1)
	{
		ft_putstr_fd("\e[32;47mheredoc >\e[0m ", 2);
		readed = read(STDIN_FILENO, heredoc, 1023);
		heredoc[readed] = '\0';
		search = ft_strnstr(heredoc, LIMITER, readed);
		if (search)
		{
			*(search--) = '\0';
			ft_putstr_fd(heredoc, file);
			break ;
		}
		ft_putstr_fd(heredoc, file);
	}
	return (close(file), 1);
}

int	main(int argc, char **argv, char **envp)
{
	(void) envp;
	if (argc < 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	read_heredoc(argv[2]);
	unlink(TEMP_PATH);
    return (EXIT_SUCCESS);
}
