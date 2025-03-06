/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:31:18 by vdurand           #+#    #+#             */
/*   Updated: 2025/03/06 18:09:42 by vdurand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	pass_untilc_syntaxed(char *str, size_t *index, char c);
static int	smart_cwords(char *str, char c);

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
		pass_untilc_syntaxed(str, &index, c);
		token = ft_substr(str, 0, index);
		if (!token)
			return (free_chartab(result), NULL);
		result[iword++] = token;
		str += index;
	}
	return (result);
}

static void	pass_untilc_syntaxed(char *str, size_t *index, char c)
{
	char	quote;

	quote = 0;
	while (str[*index])
	{
		if (!quote && (str[*index] == '"' || str[*index] == '\''))
			quote = str[*index];
		else if (quote && str[*index] == quote)
			quote = 0;
		else if (!quote && str[*index] == c)
			break ;
		(*index)++;
	}
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
		pass_untilc_syntaxed(str, &index, c);
		result += 1;
		str += index;
	}
	return (result);
}

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

char	*ft_nstrjoin(char *freable, const char *s2)
{
	char	*result;

	if (!freable)
	{
		perror("Cmd Search");
		return (NULL);
	}
	result = ft_strjoin(freable, s2);
	free(freable);
	if (!result)
		perror("Cmd Search");
	return (result);
}
