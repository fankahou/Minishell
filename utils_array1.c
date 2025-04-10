/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/10 15:41:08 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// sort array according to ascii value
/**
 * @brief Sorts an array of strings
 *
 * Sorts an array of strings by their ASCII value.
 * The sorting happens in place!
 * 
 * @param input array to sort
 *
 * @author kfan
 */
void	sort_array(char **input)
{
	char	*temp;
	int		val;
	int		y;
	int		i;

	i = 0;
	while (input[i + 1])
	{
		y = 0;
		while (input[y + 1])
		{
			val = ft_strncmp(input[y], input[y + 1],
					(ft_strlen(input[y]) | ft_strlen(input[y + 1])));
			if (val > 0)
			{
				temp = input[y];
				input[y] = input[y + 1];
				input[y + 1] = temp;
			}
			y++;
		}
		i++;
	}
}

// modified strncmp for alphabets so A = a
/**
 * @brief Compares two strings
 * 
 * Compares two strings based on their characters' alphabetical index.
 * Works like strncmp, except that A = a, B = b, etc.
 * Returns 0 if the strings are equal, otherwise it returns the
 * ASCII difference of the first different character.
 *
 * @param s1 String to compare
 * @param s2 String to compare with
 * @param n amount of characters (bytes) to compare
 * @return int 
 * @retval diff Difference between the strings
 *
 * @author kfan
 */
static int	ft_strncmp_alpha(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	str1;
	unsigned char	str2;

	i = 0;
	if (!s1 || !s2)
		return (1);
	if (n == 0)
		return (1);
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		str1 = ft_tolower(s1[i]);
		str2 = ft_tolower(s2[i]);
		if (str1 != str2)
			return (str1 - str2);
		i++;
	}
	return (0);
}

// for bonus: wildcards
// sort array according to alphabets A=a usw
void	sort_array_wildcards(char **input)
{
	char	*temp;
	int		val;
	int		y;
	int		i;

	i = 0;
	while (input[i + 1])
	{
		y = 0;
		while (input[y + 1])
		{
			val = ft_strncmp_alpha(input[y], input[y + 1],
					(ft_strlen(input[y]) | ft_strlen(input[y + 1])));
			if (val > 0)
			{
				temp = input[y];
				input[y] = input[y + 1];
				input[y + 1] = temp;
			}
			y++;
		}
		i++;
	}
}

/**
 * @brief Add quotation marks around a substring
 *
 * Adds quotation marks around a literal value inside a string.
 * The literal value is detected by the presence of an '=' sign
 * in front of it.
 * If no '=' is found it returns a duplicate of the original string.
 * 
 * @param temp String to add quotes to
 * @return char* 
 * @retval new Modified string
 *
 * @author kfan
 */
char	*attach_quote(char *temp)
{
	int		i;
	int		strlen;
	char	*new;

	i = 0;
	strlen = ft_strlen(temp);
	while (temp[i] && temp[i] != '=')
		i++;
	if (temp[i] == '=')
	{
		i++;
		new = ft_calloc(strlen + 3, 1);
		if (!new)
			return (perror("ft_calloc failed"), NULL);
		ft_strlcpy(new, temp, i + 1);
		new[i] = '\"';
		ft_strlcpy(&new[i + 1], &temp[i], strlen - i + 1);
		new[strlen + 1] = '\"';
		return (new);
	}
	return (ft_strdup(temp));
}

/**
 * @brief Debug function to print an array of strings.
 *
 * Debug function to print an array of strings.
 *
 * @param temp String array to print
 *
 * @author kfan
 */

void	print_array(char **temp)
{
	int	i;

	if (!temp)
		return ;
	i = 0;
	while (temp[i])
	{
		ft_printf("%s\n", temp[i]);
		i++;
	}
}
