/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse8_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/08 18:05:21 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//>"heyhelloworld"
// echo hey*"llow"*"rld"
// echo hey*"llaaaow"*"rld"
// echo hey****"llow"*"rld"
// echo hey****"ellow**"*"rld" this is stupid,
	// just dont put fucking stars in the quotes

// find the * in str that are not inside quotes, returns the index of char
// returns -1 if no * find >> EOF
static int	find_star(char *temp)
{
	int	quote;
	int	i;

	quote = 0;
	i = 0;
	while (temp[i])
	{
		quote = inside_quote(temp[i], quote);
		if (temp[i] == '*' && quote == 0)
			return (i);
		i++;
	}
	return (-1);
}

// filter out if prefix is present
// debugging set:
/*         printf("index = %d\n", index);
		printf("prefix = %s\n", prefix);
		printf("i = %d\n", i);
		printf("input = %s\n", input);
		printf("pin = %s\n", pin);
		printf("-----\n"); */

static int	filter_prefix(char *temp, char *input, int i, int index)
{
	char	*prefix;
	char	*pin;

	pin = input;
	prefix = ft_substr(temp, i, index);
	if (!prefix)
		return (perror("ft_substr failed"), 1);
	if (prefix[0] != '\0' && index == (int)ft_strlen(input)
		&& ft_strncmp(&input[index - ft_strlen(prefix)], prefix,
			ft_strlen(prefix))) // last prefix
		return (free(prefix), 1);
	else if (prefix[0] != '\0' && i == 0 && ft_strncmp(input, prefix,
			ft_strlen(prefix))) // first prefix
		return (free(prefix), 1);
	else if (prefix[0] != '\0') // for updating the next pin of input
	{
		pin = ft_strnstr(pin, prefix, ft_strlen(pin));
		if (!pin)
			return (free(prefix), 1);
		pin = pin + ft_strlen(prefix);
	}
	free(prefix);
	return (0);
}

// fitler out the non-wildcards files
// return 0 if it pass the filter == files that matches the condition
// check all * until in temp from begin to the end
// i is counting from temp[0] and index is counting from temp[i]
// index == 0 it is a star itself
// prefix[0] != '\0' end of file with no prefix
static int	filter(char *temp, char *input)
{
	int	index;
	int	i;

	i = 0;
	index = 0;
	while (index != (int)ft_strlen(input))
	{
		index = find_star(&temp[i]);
		if (index == -1)
			index = ft_strlen(input);
		if (index > 0 && filter_prefix(temp, input, i, index))
			return (1);
		i = i + index + 1;
	}
	return (0);
}

// opendir and get all file names into an array
// filter other hidden files starting with '.'
static char	**get_files(char *temp, char **array)
{
	DIR				*dir;
	struct dirent	*file;
	char			path[4096];
	char			**array_temp;

	dir = opendir(getcwd(path, 4096));
	if (!dir)
		return (perror("opendir failed"), NULL);
	while (1)
	{
		file = readdir(dir);
		if (!file)
			break ;
		if (file->d_name[0] != '.' && !filter(temp, file->d_name))
		{
			array_temp = add_array(array, file->d_name, 0);
			ft_free_split(array);
			if (!array_temp)
				return (closedir(dir), perror("add_array failed"), NULL);
			array = array_temp;
		}
	}
	closedir(dir);
	return (array);
}

// expand wildcards *
// get all files
// returns "*" if nothing is found
// search prefix
// for suffix need to expand $ and ignore the quotes before search...
char	*wildcards(char *temp, t_token *token, t_clean *clean)
{
	char	**array;
	char	*file;

	(void)token;
	token->wildcards = ft_strdup("*");
	if (!token->wildcards)
		return (perror("ft_strdup fail"), NULL);
	file = ft_calloc(1, 1);
	if (!file)
		return (perror("ft_calloc failed"), NULL);
	file = clean_name(temp, token, 0, file);
	if (!file)
		return (NULL);
	array = get_files(file, NULL);
	// printf("temp = %s\n", temp);
	if (array)
	{
		sort_array_wildcards(array);
		// print_array(array);
		// ft_free_split(array);
		clean->temp = array;
		free(token->wildcards);
		token->wildcards = ft_strdup("found");
	}
	free(file);
	return (token->wildcards);
}
