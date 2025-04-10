/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_is.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/10 16:08:24 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return value:
// 1 = $ no needed
// 2 = |
// 3 = <<
// 4 = <
// 5 = >
// 6 = >>
// 7 = ;
// 8 (||) and 9 (&&) for bonus

/**
 * @brief Checks which type a symbol is.
 *
 * Checks for which type the given symbol is.
 * The list of symbols and their return values are:
 * - $ = 1
 * - | = 2
 * - << = 3
 * - < = 4
 * - > = 5
 * - >> = 6
 * - ; = 7
 * - || = 8
 * - && = 9
 *
 * || and && are only needed for the bonus.
 * If no symbol is recognised, the function will return 0.
 * 
 * @param c1 First character of the symbol
 * @param c2 Second character of the symbol
 * @return int 
 * @retval type Type of the symbol
 *
 * @author kfan
 */
int	is_sym(char c1, char c2)
{
	/*     if (c1 == '|')
			return (2); */
	if (c1 == '<')
	{
		if (c2 == '<')
			return (3);
		/*         else if (c2 == '>')
					return (1); */
		else
			return (4);
	}
	if (c1 == '>')
	{
		if (c2 == '>')
			return (6);
		else
			return (5);
	}
	return (0);
}

/**
 * @brief Checks if a given character is a delimiter.
 *
 * Checks if a character is the delimiter of a command.
 * Some delimiters are made up of two characters and therefore
 * require both values to be set so to be recognised properly!
 * 
 * 
 * @param c1 first character of the delimiter
 * @param c2 second character of the delimiter (may be NULL)
 * @return int 
 * @retval delim Integer describing the delimiter.
 * - ; = 1
 * - && = 2
 * - || = 3
 * if none of these are detected, 0 is returned.
 *
 * @author kfan
 */
int	is_delimiter(char c1, char c2)
{
	if (c1 == ';')
		return (1);
	if (c1 == '&' && c2 == '&')
		return (2);
	if (c1 == '|' && c2 == '|')
		return (3);
	return (0);
}

/**
 * @brief Checks if a character is a quote.
 * 
 * Checks if the character is a quotation mark and,
 * if it is, what kind of quotation mark it is
 * (single or double).
 *
 * @param c character to check
 * @return int 
 * @retval success 1 for single quotes, 2 for double quotes, 0 otherwise.
 *
 * @author kfan
 */
int	is_quote(char c)
{
	if (c == '\'')
		return (1);
	if (c == '\"')
		return (2);
	return (0);
}

/**
 * @brief Checks if a given character is a space.
 *
 * Checks if a given character is a space character.
 * This includes ASCII indexes 9 - 13 alongside the
 * ' ' space character.
 * 
 * @param c character to check
 * @return int 
 * @retval success 1 if the character is a space, 0 otherwise.
 *
 * @author kfan
 */
int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

/**
 * @brief Checks if a given character is a pipe.
 * 
 * Checks if a given character is a '|' pipe character.
 *
 * @param c character to check
 * @return int 
 * @retval success 1 if the character is a pipe, 0 otherwise.
 *
 * @author kfan
 */
int	is_pipe(char c)
{
	if (c == '|')
		return (1);
	return (0);
}
