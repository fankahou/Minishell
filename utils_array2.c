/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/10 16:00:26 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees a partially initialised malloc'd array of strings.
 *
 * Frees a malloc'd array of malloc'd strings that hasn't been
 * fully initialised yet. All values before and at the given
 * index will be freed.
 * 
 * @param temp Array to free
 * @param i Index of the last initlialised value
 *
 * @author kfan
 */
static void	free_if_failed(char **temp, int i)
{
	while (i > 0)
		free(temp[--i]);
	free(temp);
}

/**
 * @brief Copies an array of strings, adding a prefix to each string.
 *
 * Copies an array of strings and adds the prefix "declare -x " to each
 * string. Also uses attatch_quote() on each string before adding the
 * prefix.
 * 
 * @param input Array to copy
 * @param y Counter variable, ALWAYS SET TO 0!
 * @param i Counter variable, ALWAYS SET TO 0!
 * @param j Counter variable, ALWAYS SET TO 0!
 * @return char** 
 * @retval new Malloc'd modified copy of the string array.
 *
 * @author kfan
 */
char	**copy_array_prefix(char **input, int y, int i, int j)
{
	char	**temp;
	char	*quote;

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
			return (perror("attach_quote failed"), free_if_failed(temp, i),
				NULL);
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
	int		i;

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

/**
 * @brief Duplicate an array without the value at a given index.
 *
 * Duplucates an array of strings, except for the value at a given
 * index. The excluded value will be replaced with NULL in the copy.
 * 
 * @param input array to copy
 * @param index index of value to exclude
 * @param y counter variable, ALWAYS SET TO 0!
 * @param j counter variable, ALWAYS SET TO 0!
 * @return char** 
 * @retval new Copied array
 *
 * @author kfan
 */
char	**remove_array(char **input, int index, int y, int j)
{
	char	**temp;
	int		i;

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

/**
 * @brief Adds a string to an array of strings.
 *
 * Copies an array of strings and appends the given
 * string entry to the array.
 * 
 * @param input Array to append to
 * @param entry Entry to append
 * @param y Counter variable, ALWAYS SET TO 0!
 * @return char** 
 * @retval new Appended array
 *
 * @author kfan
 */
char	**add_array(char **input, char *entry, int y)
{
	char	**temp;
	int		i;

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
