/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:41:21 by val               #+#    #+#             */
/*   Updated: 2025/02/20 14:26:37 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		return (ft_putstr_fd(ERROR_USAGE, 2), EXIT_FAILURE);
	if (!pipex(argc, argv, envp, NULL))
		return (EXIT_FAILURE);
	while (wait(NULL) > 0)
		;
	return (EXIT_SUCCESS);
}
