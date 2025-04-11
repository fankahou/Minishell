/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/11 18:07:37 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	name_count(int count, char *pin)
{
	int	quote;

	if (!pin)
		return (count);
	quote = 0;
	while (pin[count] == ' ')
		count++;
	while (pin[count] && pin[count] != ' ')
	{
		quote = inside_quote(pin[count], quote);
		if (is_sym(pin[count], pin[count + 1]) && quote == 0)
			break ;
		count++;
	}
	while (pin[count] == ' ')
		count++;
	return (count);
}

int	sym_count(char c1, char c2, char *pin)
{
	int	i;

	i = 1;
	if (c1 == '|')
	{
		while (pin[i] == ' ')
			i++;
		return (i);
	}
	if (c1 == '<')
	{
		if (c2 == '<')
			return (name_count(2, pin));
		else
			return (name_count(1, pin));
	}
	if (c1 == '>')
	{
		if (c2 == '>')
			return (name_count(2, pin));
		else
			return (name_count(1, pin));
	}
	return (0);
}

/**
 * @brief Count the length of a delimiter.
 *
 * Returns how long a delimiter is.
 * Delimiters are:
 * - ;
 * - &&
 * - ||
 * 
 * @param c1 First character of the delimiter
 * @param c2 Second character of the delimiter
 * @return int 
 * @retval len Length of the delimiter
 *
 * @author kfan
 */
int	delimiter_count(char c1, char c2)
{
	if (c1 == ';')
		return (1);
	if (c1 == '&' && c2 == '&')
		return (2);
	if (c1 == '|' && c2 == '|')
		return (2);
	return (0);
}
