/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 15:39:59 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_if_failed(char **temp, int i)
{
        while (i > 0)
            free(temp[--i]);
        free(temp);
}

char	**copy_array_prefix(char **input, int y, int i, int j)
{
	char	**temp;
    char    *quote;

    while (input[y + 1])
        y++;
    temp = malloc(sizeof(char *) * (y + 1));
	if (!temp)
		return (perror("malloc failed"), NULL);
    temp[y] = NULL;
	while (i < y)
	{
		if (input[j][0] == '_')
			j++;
        quote = attach_quote(input[j]);
        if (!quote)
            return (perror("attach_quote failed"), free_if_failed(temp, i), NULL);
        temp[i] = ft_strjoin("declare -x ", quote);
        free(quote);
		if (!temp[i])
            return (free_if_failed(temp, i), perror("ft_strjoin failed"), NULL);
		i++;
		j++;
	}
	return (temp);
}

/**
 * @brief Copies an array of strings.
 *
 * Copies an array of strings and returns
 * the malloc'd copy.
 * Currently only used to copy envp.
 * 
 * @param input 
 * @return char** 
 * @retval temp Malloc'd copy of the array.
 *
 * @author kfan
 */
char	**copy_array(char **input)
{
	char	**temp;
	int		y;
    int     i;

    y = 0;
    while (input[y])
        y++;
    temp = malloc(sizeof(char *) * (y + 1));
	if (!temp)
		return (perror("malloc failed"), NULL);
    temp[y] = NULL;
	i = 0;
	while (i < y)
	{
		temp[i] = ft_strdup(input[i]);
		if (!temp[i])
            return (free_if_failed(temp, i), perror("ft_strdup failed"), NULL);
		i++;
	}
	return (temp);
}

char	**remove_array(char **input, int index, int y, int j)
{
	char	**temp;
    int     i;

    while (input[y])
        y++;
    temp = malloc(sizeof(char *) * (y));
	if (!temp)
		return (perror("malloc failed"), NULL);
    temp[y - 1] = NULL;
	i = 0;
	while (j < y - 1)
	{
        if (i == index)
            i++;
		temp[j] = ft_strdup(input[i]);
		if (!temp[j])
            return (free_if_failed(temp, j), perror("ft_strdup failed"), NULL);
		i++;
        j++;
	}
	return (temp);
}

char	**add_array(char **input, char *entry, int y)
{
	char	**temp;
    int     i;

    while (input && input[y])
        y++;
    temp = malloc(sizeof(char *) * (y + 2));
	if (!temp)
		return (perror("malloc failed"), NULL);
    temp[y + 1] = NULL;
    i = 0;
	while (i < y)
	{
		temp[i] = ft_strdup(input[i]);
		if (!temp[i])
            return (free_if_failed(temp, i), perror("ft_strdup failed"), NULL);
		i++;
	}
	temp[y] = ft_strdup(entry);
	if (!temp[y])
        return (perror("ft_strdup failed"), NULL);
	return (temp);
}
