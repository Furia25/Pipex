/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:46:50 by val               #+#    #+#             */
/*   Updated: 2025/01/24 15:25:26 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft.h"
# include "ft_printf.h"
# include "unistd.h"
# include <sys/types.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
//
# define TEMP_PATH	"./.tmp_heredoc"
# define ERROR_USAGE	"Error: Not enough arguments. Usage: pipex <argument1> <argument2> ...\n"
//
#endif