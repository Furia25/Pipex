/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/02/24 13:23:23 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_temp_heredoc(char *name);
static void	rm_file(char *name);

int	main(int argc, char **argv, char **envp)
{
	char	hd_name[15];
	int		exit_code;

	hd_name[0] = '\0';
	if (argc < 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			return (ft_putstr_fd(ERROR_HEREDOC_USAGE, 2), EXIT_FAILURE);
		if (!get_temp_heredoc(hd_name))
			return (perror("Here_doc File"), EXIT_FAILURE);
		if (!read_heredoc(argv[2], hd_name))
			exit(EXIT_FAILURE);
		exit_code = pipex(argc, argv, envp, hd_name);
	}
	else
		exit_code = pipex(argc, argv, envp, NULL);
	while (wait(NULL) > 0)
		;
	if (hd_name[0] != '\0')
		rm_file(hd_name);
	return (exit_code);
}

static int	get_temp_heredoc(char *name)
{
	int				i;
	int				fd;
	unsigned char	rand_bytes[14];

	if (access(SYSRANDOM, F_OK | R_OK) != 0)
		return (0);
	fd = open(SYSRANDOM, O_RDONLY);
	if (fd == -1)
		return (0);
	if (read(fd, rand_bytes, 14) <= 0)
		return (0);
	close(fd);
	i = 0;
	while (i < 14)
	{
		name[i] = 'a' + (rand_bytes[i] % 26);
		i++;
	}
	name[0] = '.';
	name[1] = 't';
	name[2] = 'm';
	name[3] = 'p';
	name[4] = '_';
	name[14] = '\0';
	return (1);
}

static void	rm_file(char *name)
{
	if (access(name, W_OK | F_OK) == 0)
		unlink(name);
}
