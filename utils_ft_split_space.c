/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ft_split_space.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/04/23 13:54:53 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of spaces in a string.
 *
 * Counts the number of spaces inside a string.
 * Spaces inside quotation marks are ignored and
 * therefore not counted.
 *
 * @author kfan
 * 
 * @param s string to analyze
 * @return size_t 
 * @retval count Number of spaces in s.
 */
static size_t	ft_count(char *s)
{
	size_t	i;
	size_t	count;
	int		quote;

	i = 0;
	count = 0;
	quote = 0;
	if (s[i] != '\0' && !is_space(s[i]))
		count++;
	while (s[i])
	{
		quote = inside_quote(s[i], quote);
		if (is_space(s[i]) && quote == 0)
		{
			while (is_space(s[i]))
				i++;
			if (s[i] != '\0' && !is_space(s[i]))
				count++;
		}
		else
			i++;
	}
	return (count);
}

/**
 * @brief Returns a substring delimited by spaces.
 *
 * Takes a string s and returns a substring starting
 * at s[0], ending at the first spae character that
 * is not withing " double or ' single quotes.
 *
 * @author kfan
 * 
 * @param s string to get substing of
 * @return char* 
 * @retval str substring
 */
static char	*ft_newstring(char *s)
{
	size_t	i;
	char	*str;
	int		quote;

	i = 0;
	quote = 0;
	while (s[i] && !is_space(s[i]))
	{
		quote = inside_quote(s[i], quote);
		i++;
		while (s[i] && quote != 0)
			quote = inside_quote(s[i++], quote);
	}
	str = malloc(i + 1);
	if (!str)
		return (NULL);
	str[i] = '\0';
	while (i > 0)
	{
		i--;
		str[i] = s[i];
	}
	return (str);
}

/**
 * @brief Splits a string on spaces.
 * 
 * Splits a string on spaces, unless they are inside
 * quotation marks.
 *
 * Main logic for ft_split_space.
 * 
 * @param s String to split
 * @param array Array to write result to
 * @return char** 
 * @retval array Resulting array
 */
static char	**ft_array(char *s, char **array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_count(s))
	{
		while (s[j] && is_space(s[j]))
			j++;
		array[i] = ft_newstring(s + j);
		if (!array[i])
		{
			ft_free(array, i - 1);
			return (NULL);
		}
		j = j + ft_strlen(array[i]);
		i++;
	}
	return (array);
}

/**
 * @brief Split a string on spaces.
 *
 * Splits a string into an array of strings on spaces.
 * Does not split on spaces taht are between " double
 * or ' single quotes.
 *
 * Example:
 * The string "This is 'a fancy string' string!" will
 * be split into "This", "is", "a fancy string", "string!".
 *
 * Bulk of the logic is in ft_array.
 *
 * @author kfan
 * 
 * @param s string to split
 * @return char** 
 * @retval array Array of substrings.
 */
char	**ft_split_space(char *s)
{
	char	**array;

	if (!s)
		return (NULL);
	array = malloc(sizeof(char *) * (ft_count(s) + 1));
	if (!array)
		return (NULL);
	array[ft_count(s)] = NULL;
	array = ft_array(s, array);
	return (array);
}
