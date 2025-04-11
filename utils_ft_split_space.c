/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ft_split_space.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/04/11 16:47:43 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new: will not split if inside quotation
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

// new: will skip space if inside quotation
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
