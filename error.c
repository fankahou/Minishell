/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 19:54:31 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//write all error messages to a file and print it out later?
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
 * @param str message printed
 * @param token token in which the error occurred
 *
 * @author kfan
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
 * "\033[1;41m ERROR \033[22;0m -> ". Uses perror to print the message.
 *
 * @param msg message to print, may be NULL
 * @return int
 * @retval return always returns 1
 *
 * @author kmautner
 */
int	error(char *msg)
{
	char *prefix;
	char *combined;

	prefix = "\033[1;41m ERROR \033[22;0m -> ";
	combined = ft_strjoin(prefix, msg);
	if (msg)
	{
		perror(combined);
		free(combined);
	}
	else
		perror("\033[1;41m ERROR \033[22;0m -> unknown error (no message)");
	return (1);
}

/**
 * @brief Prints a warning to stdout.
 *
 * This function prints a warning message to the standard output
 * (fd = 1). All messages are prepended with "\033[1;43m WARNING \033[22;0m -> "
 * and terminated with a newline.
 *
 * @param msg message to print, may be NULL
 *
 * @author kmautner
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
 * @param msg message to print
 *
 * @author kmautner
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
