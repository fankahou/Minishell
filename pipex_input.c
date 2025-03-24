/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 15:27:40 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (execve(path, cmds->cmd, token->envp) == -1)
	{
		if ((!ft_strncmp(cmds->cmd[0], "../", 3) && ft_strlen(cmds->cmd[0]) == 3) || (!ft_strncmp(cmds->cmd[0], "./", 2) && ft_strlen(cmds->cmd[0]) == 2))
		{
			if (ft_strlen(cmds->cmd[0]) == 2)
				perror("minishell: ./: Is a directory");
			if (ft_strlen(cmds->cmd[0]) == 3)
				perror("minishell: ../: Is a directory");
			exit (126);
		}
		perror("minishell: command not found");
		exit (127);
	}
}

static void	parent(int *fd, t_cmds *cmds, int pid, t_token *token)
{
	int	status;

	status = 0;
	if (ft_strncmp(cmds->infile, "/dev/urandom", 12)
		&& ft_strncmp(cmds->infile, "/dev/random", 11)
		&& ft_strncmp(cmds->cmd[1], "/dev/urandom", 12) 
		&& ft_strncmp(cmds->cmd[1], "/dev/random", 11))// wait for signal?
	{
		waitpid(pid, &status, 0); // protection?
	}
	else
		kill(pid, 1); // usleep or kill later?
	if (fd)
	{
		close(fd[1]);
		if (dup2(fd[0], 0) == -1)
		{
			close(fd[0]);
			perror("dup2 failed");
			exit(1);
		}
		close(fd[0]);
	}
	token->exit_code[0] = WEXITSTATUS(status);
		// if statement for the one terminated by signal also?
}

/* // new: not neccesary in pipex but in minishell!
static int replace_arg(char *cmd, char **temp)
{
	int i;
	char *new_arg;

	i = 0;
	while (cmd[i] != ' ' && cmd[i] != '\0')
		i++;
	if (cmd[i] == '\0')
		return (0);
	i++;
	// check for flags?
	new_arg = ft_strdup(&cmd[i]);
	printf("new = %s\n", new_arg);
	if (!new_arg)
		return(perror("ft_strdup failed"), 1);
	if (temp[1])
		free(temp[1]);
	temp[1] = new_arg;
	i = 2;
	while(temp[i])
	{
		free(temp[i]);
		temp[i] = NULL;
		i++;
	}
	return(0);
} */

int	input(t_cmds *cmds, t_token *token)
{
	int		pid;
	int		fd[2];
	char *path;

	//check for build ins!
	if (pipe(fd) == -1)
		return(perror("pipe failed"), 1);
	// handle flags??? eg."       ls         -l     -a"
/* 	if (temp[1] && temp[1][0] != '\0' && replace_arg(cmd, temp))  // only for echo??? otherwise will kill those flags?
		return (1); */
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return(perror("fork failed"), 1);
	}
	path = get_path(cmds->cmd, token->envp);
	if (!path)
		return (1);
	if (pid == 0)
		child(fd, cmds, token, path);
	else
		parent(fd, cmds, pid, token);
	free(path);
	return (0);
}

int	last_input(t_cmds *cmds, t_token *token)
{
	int		pid;
	char *path;

	//check for build ins!
/* 	if (temp[1] && temp[1][0] != '\0' && replace_arg(cmd, temp)) // only for echo??? echo    "test        321"
		return (1); */
	pid = fork();
	if (pid == -1)
		return(perror("fork failed"), 1);
	path = get_path(cmds->cmd, token->envp);
	if (!path)
		return (1);
	if (pid == 0)
		child(NULL, cmds, token, path);
	else
		parent(NULL, cmds, pid, token);
	free(path);
	return (0);
}
