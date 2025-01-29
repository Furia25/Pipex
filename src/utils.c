/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdurand <vdurand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:31:18 by vdurand           #+#    #+#             */
/*   Updated: 2025/01/29 18:05:57 by vdurand          ###   ########.fr       */
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

t_list	*smart_split(char *str, char c)
{
	t_list	*result;
	t_list	*temp;
	char	*token;
	size_t	index;

	result = ft_lstnew(NULL);
	if (!result)
		return (NULL);
	while (*str)
	{
		while (*str && *str == c)
            str++;
		if (*str == '\0')
            break;
		index = 0;
		while(str[index] && str[index] != c)
			index++;
		token = ft_substr(str, 0, index);
		if (!token)
			return (ft_lstclear(&result, free), NULL);
		temp = ft_lstnew(token);
		if (!temp)
			return (free(token), ft_lstclear(&result, free), NULL);
		ft_lstadd_back(&result, temp);
		str += index;
	}
	return (result);
}
