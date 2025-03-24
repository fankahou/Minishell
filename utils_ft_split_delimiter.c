/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ft_split_delimiter.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/03/20 11:54:20 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new: will not split if inside quotation
static size_t	ft_count(char const *s)
{
	size_t	i;
	size_t	count;
	int		quote;

	i = 0;
	count = 0;
	quote = 0;
	if (s[i] != '\0' && !is_delimiter(s[i],s[i + 1]))
		count++;
	while (s[i])
	{
		quote = inside_quote(s[i], quote);
		if (is_delimiter(s[i],s[i + 1]) && quote == 0)
		{
			count++;
			i = i + delimiter_count(s[i],s[i + 1]);
			if (!is_delimiter(s[i],s[i + 1]) && s[i] != '\0')
				count++;
		}
		else
			i++;
	}
	return (count);
}

void	ft_free(char **array, int i)
{
	while (i >= 0)
	{
		free(array[i]);
		array[i] = NULL;
		i--;
	}
	free(array);
	array = NULL;
}

// new: will skip space if inside quotation
static char	*ft_newstring(char const *s)
{
	size_t	i;
	char	*str;
	int		quote;

	i = 0;
	quote = 0;
	while (s[i] && !is_delimiter(s[i],s[i + 1]))
	{
		quote = inside_quote(s[i], quote);
		i++;
		while (s[i] && quote != 0)
			quote = inside_quote(s[i++], quote);
	}
	if (is_delimiter(s[i],s[i + 1]) && i == 0)
		i = i + delimiter_count(s[i],s[i + 1]);
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

static char	**ft_array(char const *s, char **array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < ft_count(s))
	{
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

char	**ft_split_delimiter(char const *s)
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

/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	char	**ptr;
	size_t		i;

	(void)argc;
	ptr = ft_split(argv[1], argv[2][0]);
	i = 0;
	//printf("%zu\n", ft_count(argv[1], argv[2][0]));
	while (i < ft_count(argv[1], argv[2][0]))
	{
		printf("%s\n", ptr[i]);
		i++;
	}
	return (0);
}
*/
