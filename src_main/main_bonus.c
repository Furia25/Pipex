/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/02/10 19:17:39 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	index;

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
	if (!pipex(argc, argv, envp, index))
		return (EXIT_FAILURE);
	while (wait(NULL) > 0)
		;
	if (access(TEMP_PATH, W_OK | F_OK) == 0)
		unlink(TEMP_PATH);
	return (EXIT_SUCCESS);
}
