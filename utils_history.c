/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:06:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/04/23 13:24:28 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if two strings are equal.
 *
 * This function checks if two strings are equal.
 * They are equal if:
 * - They have the same length
 * - They have the same characters in the same positions
 *
 * @author kmautner
 *
 * @param str1 first string
 * @param str2 second string
 * @return int
 * @retval success 0 if the strings are equal, 1 otherwise.
 */
int	str_equals(char *str1, char *str2)
{
	size_t	len;

	len = ft_strlen(str1);
	if (len != ft_strlen(str2))
		return (0);
	if (ft_strncmp(str1, str2, len) != 0)
		return (0);
	return (1);
}

/**
 * @brief Shifts an array to the left.
 *
 * Shifts the contents of an array of strings to the left (towards
 * index 0) by one position.
 * All elements should be malloc'd!
 * The first element will get freed and overwritten!
 *
 * @author kmautner
 *
 * @param array array to shift
 * @param length length of the array
 */
void	shift_array(char **array, int length)
{
	int	c;

	if (!array || length == 0)
		return ;
	free(array[0]);
	c = 1;
	while (c < length)
	{
		array[c - 1] = array[c];
		c++;
	}
}
