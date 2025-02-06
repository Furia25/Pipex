/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:46:50 by val               #+#    #+#             */
/*   Updated: 2025/02/06 15:46:25 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include "ft_printf.h"
# include "unistd.h"
# include <sys/types.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/types.h>

//
# define TEMP_PATH	"./.tmp_heredoc"
# define ERROR_USAGE	"Error: Invalid Arguments. \
	Usage: pipex <infile> <argument1> <argument2> <outfile>...\n"
# define ERROR_HEREDOC_USAGE	"Error: Not enough arguments. \
	Usage: pipex here_doc <limiter> <argument1> <argument2> <outfile>...\n"
//
int		open_file(char *path, int mode, int index);
void	free_chartab(char **tab);
char	**smart_split(char *str, char c);
char	*ft_nstrjoin(char *freable, const char *s2);
int		open_file(char *path, int mode, int index);
int		read_heredoc(char *limiter);
int		pipex(int argc, char *argv[], char *envp[], int index);

#endif