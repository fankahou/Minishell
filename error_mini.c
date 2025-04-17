/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_mini.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/17 14:11:02 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new: all small errors that only stop the current pipe but continues

/**
 * @brief Indicates a mini error.
 *
 * This function indicates a mini error in the
 * token that is currently being processed and
 * will not stop the processing of this token.
 *
 * @param str message printed
 * @param token token in which the error occurred
 *
 * @author kfan
 */
void	mini_error(char *str, char *arg, t_token *token, int *fd)
{
	write(2, "minishell: ", 11);
	if (arg)
	{
		write(2, arg, ft_strlen(arg));
		write(2, ": ", 2);
	}
	if (str)
		write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	if (token)
		token->exit_code[0] = 1;
	if (fd)
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}

/**
 * @brief Raises a command not found error.
 *
 * Raises a command not found error, writing an error
 * message to stdout (fd = 2). The message is formatted as
 * follows "<cmd>: command not found".
 * 
 * @param cmd command that caused the error (may be NULL)
 *
 * @author kfan
 */
static void	command_not_found(char *cmd)
{
	if (cmd)
		write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	perror("command not found");
}

/**
 * @brief Raises an execve error
 *
 * Raises an execve error and prints an error message
 * to stdout (fd = 2). Depending on the type of
 * execve error (permission denied or command not found)
 * the printing is handled by mini_error() or command_not_found().
 * 
 * @param cmd command that caused the error
 * @param path path to the faulty executable
 * @param token token in which the error occurred
 * @param dir directory in which the error occurred
 *
 * @ref mini_error
 * @ref command_not_found
 *
 * @author kfan
 */
void	execve_error(char *cmd, char *path, t_token *token, DIR *dir)
{
	token->error[0] = 2;
	token->exit_code[0] = 127;
	if (cmd)
		dir = opendir(cmd);
	if (!dir)
	{
		if (cmd && access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
		{
			mini_error("Permission denied", cmd, NULL, NULL);
			token->exit_code[0] = 126;
		}
		else
			command_not_found(cmd);
	}
	else
	{
		closedir(dir);
		if (!ft_strncmp(cmd, "..", ft_strlen(path)) && ft_strlen(path) > 2)
			command_not_found(cmd);
		else
		{
			mini_error("Is a directory", cmd, NULL, NULL);
			token->exit_code[0] = 126;
		}
	}
}

/**
 * @brief Indicates an open error.
 *
 * This function indicates an open error in the
 * token that is currently being processed and
 * stops the processing of this token.
 *
 * @param str debug feature, unused
 * @param token token in which the error occurred.
 *
 * @author kfan
 */
void	open_error(char *str, t_token *token, int *fd, int k)
{
	write(2, "minishell: ", 11);
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
	}
	perror(NULL);
	if (token)
		token->cmds[k]->exit_code = 1;
	if (fd)
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}
