/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/02/24 13:23:31 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	exit_code;

	if (argc != 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	exit_code = pipex(argc, argv, envp, NULL);
	while (wait(NULL) > 0)
		;
	return (exit_code);
}
