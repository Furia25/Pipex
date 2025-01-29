/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:31:18 by vdurand           #+#    #+#             */
/*   Updated: 2025/01/29 18:29:07 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_chartab(char **tab)
{
	size_t	index;

	index = 0;
	while (tab[index])
	{
		free(tab[index]);
		index++;
	}
	free(tab);
}

static int	smart_cwords(char *str, char c)
{
	int		result;
	size_t	index;

	result = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str == '\0')
			break ;
		index = 0;
		while(str[index] && str[index] != c)
			index++;
		result += 1;
		str += index;
	}
	return (result);
}

char	**smart_split(char *str, char c)
{
	char	**result;
	char	*token;
	size_t	iword;
	size_t	index;

	result = ft_calloc(smart_cwords(str, c) + 1, sizeof(char *));
	if (!result)
		return (NULL);
	iword = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str == '\0')
			break ;
		index = 0;
		while(str[index] && str[index] != c)
		{
			if (str[index] == '"')
			{
				index++;
				while (str[index] && str[index] != '"')
					index++;
			}
			index++;
		}
		token = ft_substr(str, 0, index);
		if (!token)
			return (free_chartab(result), NULL);
		result[iword++] = token;
		str += index;
	}
	return (result);
}
