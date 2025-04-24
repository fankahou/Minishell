/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_mini.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/24 18:24:14 by kfan             ###   ########.fr       */
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
 * @author kfan
 *
 * @param str message printed
 * @param token token in which the error occurred
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
 * @author kfan
 *
 * @param cmd command that caused the error (may be NULL)
 */
static void	command_not_found(char *cmd, t_cmds *cmds)
{
	int	i;

	i = 0;
	if (cmd)
		write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	while (cmd[i] && cmd[i] != '/')
		i++;
	if (cmd[i] == '/' || cmds->path_flag == 1)
		write(2, "No such file or directory\n", 26);
	else
		write(2, "command not found\n", 18);
}

/**
 * @brief Raises an execve error
 *
 * Raises an execve error and prints an error message
 * to stdout (fd = 2). Depending on the type of
 * execve error (permission denied or command not found)
 * the printing is handled by mini_error() or command_not_found().
 *
 * @author kfan
 *
 * @param cmd command that caused the error
 * @param path path to the faulty executable
 * @param token token in which the error occurred
 * @param dir directory in which the error occurred
 */
void	execve_error(t_cmds *cmds, char *path, t_token *token, DIR *dir)
{
	if (cmds->cmd && cmds->cmd[0])
		dir = opendir(cmds->cmd[0]);
	if (!dir)
	{
		if (cmds->path_flag != 0 && access(cmds->cmd[0], F_OK) == 0
			&& access(cmds->cmd[0], X_OK) != 0)
		{
			mini_error("Permission denied", cmds->cmd[0], NULL, NULL);
			token->exit_code[0] = 126;
		}
		else
			command_not_found(cmds->cmd[0], cmds);
	}
	else
	{
		closedir(dir);
		if (!ft_strncmp(cmds->cmd[0], "..", ft_strlen(path))
			&& ft_strlen(path) > 2)
			command_not_found(cmds->cmd[0], cmds);
		else
		{
			mini_error("Is a directory", cmds->cmd[0], NULL, NULL);
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
 * @author kfan
 *
 * @param str debug feature, unused
 * @param token token in which the error occurred.
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

// ambiguous redirect error
void	amb_error(char *str, t_token *token, int *fd, int k)
{
	write(2, "minishell: ", 11);
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
	}
	write(2, "ambiguous redirect\n", 19);
	if (token)
		token->cmds[k]->exit_code = 1;
	if (fd)
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}
