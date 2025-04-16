/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/16 16:31:39 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exit code will not change if exit() is not executed
static int check_builtins(t_cmds *cmds, t_token *token, int *fd)
{
	if (builtins_pipe_fd_out(cmds, fd))
		return (1);
	if (str_equals(cmds->cmd[0], "echo"))
		cmds->exit_code = builtins_echo(&cmds->cmd[1]);
	else if (str_equals(cmds->cmd[0], "cd"))
		cmds->exit_code = builtins_cd(&cmds->cmd[1], token->envp, token);
	else if (str_equals(cmds->cmd[0], "pwd"))
		cmds->exit_code = builtins_pwd(&cmds->cmd[1]);
	else if (str_equals(cmds->cmd[0], "export"))
		cmds->exit_code = builtins_export(&cmds->cmd[1], token, 0, 0);
	else if (str_equals(cmds->cmd[0], "unset"))
		cmds->exit_code = builtins_unset(&cmds->cmd[1], token->envp, token, 0);
	else if (str_equals(cmds->cmd[0], "env"))
		cmds->exit_code = builtins_env(token->envp, &cmds->cmd[1]);
	else if (str_equals(cmds->cmd[0], "exit"))
	{
		builtins_exit(&cmds->cmd[1], token);
		cmds->exit_code = token->exit_code[0];
	}
	else
		return (0);
	if (builtins_pipe_fd_in(cmds, fd))
		return (1);
	return (1);
}

// user readdir()?
static void	child(int *fd, t_cmds *cmds, t_token *token, char *path)
{
	if (fd)
	{
		close(fd[0]);
		if (!cmds->outfile && dup2(fd[1], 1) == -1)
		{
			close(fd[1]);
			perror("dup2 failed");
			exit(1);
		}
		close(fd[1]);
	}
	if (path)
		execve(path, cmds->cmd, token->envp);
	execve_error(cmds->cmd[0], path, token, NULL);
}

static void	parent(int *fd)
{
	if (fd)
	{
		close(fd[1]);
		if (dup2(fd[0], 0) == -1)
		{
			close(fd[0]);
			perror("dup2 failed");
			return ;
		}
		close(fd[0]);
	}
}

int	input(t_cmds *cmds, t_token *token)
{
	int		fd[2];
	char *path;

	path = NULL;
	if (pipe(fd) == -1)
		return(perror("pipe failed"), 1);
	if (cmds->fd[0] == -1)
		return (empty_pipe(fd));
	if (check_builtins(cmds, token, fd))
		return (0);
	if (cmds->cmd[0])
		path = get_path(cmds->cmd, token->envp);
	cmds->pid = fork();
	if (cmds->pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return(perror("fork failed"), 1);
	}
	if (cmds->pid == 0)
		child(fd, cmds, token, path);
	else
		parent(fd);
	if (path)
		free(path);
	return (0);
}

int	last_input(t_cmds *cmds, t_token *token)
{
	char *path;

	path = NULL;
	if (check_builtins(cmds, token, NULL))
		return (0);
	if (cmds->cmd[0])
		path = get_path(cmds->cmd, token->envp);
	cmds->pid = fork();
	if (cmds->pid == -1)
		return(perror("fork failed"), 1);
	if (cmds->pid == 0)
		child(NULL, cmds, token, path);
	else
		parent(NULL);
	if (path)
		free(path);
	return (0);
}
