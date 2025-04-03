/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_mini.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 19:55:20 by kfan             ###   ########.fr       */
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

static void command_not_found(char *cmd)
{
	if(cmd)
		write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	perror("command not found");
}

void	execve_error(char *cmd, char *path, t_token *token, DIR *dir)
{	
	token->error[0] = 2;
	token->exit_code[0] = 127;
	dir = NULL;
	if (cmd)
		dir = opendir(cmd);
	if (!dir)
	{
		if (cmd && access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0)
		{
			mini_error("Permission denied", cmd, NULL, NULL);
			token->exit_code[0] = 126;
		}
/* 		else if (access(cmd, F_OK) == 0 && access(cmd, R_OK) == 0)
			printf("file exists"); */ // recursion make tree for cmd? open and gnl? implement -c for minishell and pass to it!
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
void	open_error(char *str, t_token *token, int *fd)
{
	write(2, "minishell: ", 11);
	if (str)
	{
		write(2, str, ft_strlen(str));
		write(2, ": ", 2);
	}
	perror(NULL);
	if (token)
		token->exit_code[0] = 1;
	if (fd)
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}
