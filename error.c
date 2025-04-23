/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/23 16:30:40 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// write all error messages to a file and print it out later?
// open(fd);
// dup2(fd, 2)?
// in bash all errors prints out only after the STDOUT

/**
 * @brief Indicates a syntax error.
 *
 * This function indicates a syntax error in the
 * token that is currently being processed and
 * stops the processing of this token.
 *
 * @author kfan
 *
 * @param str message printed
 * @param token token in which the error occurred
 */
void	syntax_error(char *str, t_token *token)
{
	write(2, "minishell: syntax error: \"", 26);
	if (str)
		write(2, str, ft_strlen(str));
	write(2, "\"\n", 2);
	if (token)
	{
		token->exit_code[0] = 2;
		token->error[0] = 1;
	}
}

/**
 * @brief Prints an error to stderr and returns 1.
 *
 * This function prints an error message to the standard error
 * and returns 1. All messages are prepended with
 * "\033[1;41m ERROR \033[22;0m -> ".
 * Uses write() to print the message.
 *
 * @author kmautner
 *
 * @param msg message to print, may be NULL
 * @return int
 * @retval return Always returns 1.
 */
int	error(char *msg)
{
	write(2, "\033[1;41m ERROR \033[22;0m -> ", 25);
	if (msg)
		write(2, msg, ft_strlen(msg));
	else
		write(2, "unknown error (no message)", 26);
	write(2, "\n", 1);
	return (1);
}

/**
 * @brief Prints an error to stderr and returns 1.
 *
 * This function prints an error message to the standard error
 * (fd = 2) and returns 1. The message is prepended with
 * "\033[1;41m ERROR \033[22;0m -> " and the arguments are separated
 * by a ':' colon.
 * Uses write() to print the messages.
 *
 * @author kmautner
 *
 * @param msg
 * @param msg2
 * @return int
 * @retval return Always returns 1.
 */
int	error2(char *msg, char *msg2)
{
	write(2, "\033[1;41m ERROR \033[22;0m -> ", 25);
	if (msg)
		write(2, msg, ft_strlen(msg));
	else
		write(2, "unknown error (no message)", 26);
	if (msg2)
		write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (1);
}

/**
 * @brief Prints a warning to stdout.
 *
 * This function prints a warning message to the standard output
 * (fd = 1). All messages are prepended with "\033[1;43m WARNING \033[22;0m -> "
 * and terminated with a newline.
 *
 * @author kmautner
 *
 * @param msg message to print, may be NULL
 */
void	warn(char *msg)
{
	write(1, "\033[1;43m WARNING \033[22;0m -> ", 28);
	if (msg)
		write(1, msg, ft_strlen(msg));
	else
		write(1, "unknown warning (no message)", 28);
	write(1, "\n", 1);
}

/**
 * @brief Prints a debug message to stdout.
 *
 * This function prints a debug message to the standard output
 * (fd = 1). All messages are prepended with "\033[1;44m DEBUG \033[22;0m -> "
 * and terminated with a newline.
 * DEBUG must be defined as 1 for this function to execute,
 * otherwise it will not do anything!
 *
 * @author kmautner
 *
 * @param msg message to print
 */
void	debug(char *msg)
{
	if (!DEBUG)
		return ;
	write(1, "\033[1;44m DEBUG \033[22;0m -> ", 25);
	if (msg)
		write(1, msg, ft_strlen(msg));
	else
		write(1, "no message given", 16);
	write(1, "\n", 1);
}
