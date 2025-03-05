/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/03/05 15:28:08 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	exit_code;
	int	exit_status;
	int	status;

	if (argc != 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	exit_code = pipex(argc, argv, envp, NULL);
	while (wait(&status) > 0)
	{
		exit_status = (status >> 8) & 0xFF;
		if (exit_status != EXIT_SUCCESS)
			return (exit_status);
	}
	return (exit_code);
}
