/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/08 18:13:46 by kmautner         ###   ########.fr       */
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
	while (pin[count] == ' ') // new: skip space after syntax
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
			i++; // new: skip space after syntax
		return (i);
	}
	if (c1 == '<')
	{
		if (c2 == '<' /*  || c2 == '>' */)
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
