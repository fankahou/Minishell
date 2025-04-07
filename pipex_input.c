/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/07 18:42:03 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// exit code will not change if exit() is not executed
static int check_builtins(t_cmds *cmds, t_token *token, int *fd)
{
	//printf("cmd = %s----\n", cmds->cmd[0]);
	if (builtins_pipe_fd_out(cmds, fd))
		return (1);
	if ((!ft_strncmp(cmds->cmd[0], "echo", 4) && ft_strlen(cmds->cmd[0]) == 4))
		token->exit_code[0] = builtins_echo(&cmds->cmd[1]);
	else if ((!ft_strncmp(cmds->cmd[0], "cd", 2) && ft_strlen(cmds->cmd[0]) == 2))
		token->exit_code[0] = builtins_cd(&cmds->cmd[1], token->envp, token);
	else if ((!ft_strncmp(cmds->cmd[0], "pwd", 3) && ft_strlen(cmds->cmd[0]) == 3))
		token->exit_code[0] = builtins_pwd(token->envp);
	else if ((!ft_strncmp(cmds->cmd[0], "export", 6) && ft_strlen(cmds->cmd[0]) == 6))
		token->exit_code[0] = builtins_export(&cmds->cmd[1], token, 0, 0);
	else if ((!ft_strncmp(cmds->cmd[0], "unset", 5) && ft_strlen(cmds->cmd[0]) == 5))
		token->exit_code[0] = builtins_unset(&cmds->cmd[1], token->envp, token, 0);
	else if ((!ft_strncmp(cmds->cmd[0], "env", 3) && ft_strlen(cmds->cmd[0]) == 3))
		token->exit_code[0] = builtins_env(token->envp);
	else if ((!ft_strncmp(cmds->cmd[0], "exit", 4) && ft_strlen(cmds->cmd[0]) == 4))
		builtins_exit(&cmds->cmd[1], token);
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

static void	parent(int *fd, t_cmds *cmds, t_token *token, char *path)
{
	int	status;

	status = 0;
	(void)path;
	(void)cmds;
	//dont wait here!
	//waitpid(cmds->pid, &status, 0); // protection?
/* 	if (cmds->cmd[0] && cmds->cmd[1])
		path = cmds->cmd[1];
	if (ft_strncmp(cmds->infile, "/dev/urandom", 12)
		&& ft_strncmp(cmds->infile, "/dev/random", 11)
		&& ft_strncmp(path, "/dev/urandom", 12) 
		&& ft_strncmp(path, "/dev/random", 11))// wait for signal?
	{
		waitpid(cmds->pid, &status, 0); // protection?
	}
	else
		kill(cmds->pid, 1); // usleep or kill later? */
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
	token->exit_code[0] = WEXITSTATUS(status);
		// if statement for the one terminated by signal also?
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
		parent(fd, cmds, token, path);
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
/* 	if (temp[1] && temp[1][0] != '\0' && replace_arg(cmd, temp)) // only for echo??? echo    "test        321"
		return (1); */
	if (cmds->cmd[0])
		path = get_path(cmds->cmd, token->envp);
	cmds->pid = fork();
	if (cmds->pid == -1)
		return(perror("fork failed"), 1);
	if (cmds->pid == 0)
		child(NULL, cmds, token, path);
	else
		parent(NULL, cmds, token, path);
	if (path)
		free(path);
	return (0);
}
