/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:46:50 by val               #+#    #+#             */
/*   Updated: 2025/03/06 18:10:14 by vdurand          ###   ########.fr       */
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

typedef struct s_pinfo
{
	int	is_last;
	int	outfile_fd;
}		t_pinfo;

//
# define SYSRANDOM	"/dev/urandom"
# define ERROR_USAGE	"Error: Invalid Arguments. \
	Usage: pipex <infile> <argument1> <argument2> <outfile>...\n"
# define ERROR_HEREDOC_USAGE	"Error: Not enough arguments. \
	Usage: pipex here_doc <limiter> <argument1> <argument2> <outfile>...\n"
//
void	free_chartab(char **tab);
char	**smart_split(char *str, char c);
char	*ft_nstrjoin(char *freable, const char *s2);
int		read_heredoc(char *limiter, char *file_name);
int		pipex(int argc, char *argv[], char *envp[], char *name);
int		exec_pipe(char *cmd, char **envp, int *lfd, t_pinfo info);
int		full_return(char *str, int code, int fd1, int fd2);

#endif