/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/12 19:28:32 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_pipex(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			free(array[i]);
			array[i] = NULL;
			i++;
		}
		free(array);
		array = NULL;
	}
}

static char	*find_bin(char **cmd, char **paths, int i)
{
	char	*temp;
	char	*temp1;

	while (cmd && cmd[0] && paths[i] && ft_strncmp(cmd[0], "./", 2))
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (perror("strjoin1 failed"), NULL);
		temp1 = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (!temp1)
			return (perror("strjoin2 failed"), NULL);
		if (access(temp1, F_OK) == 0)
			return (temp1);
		free(temp1);
		i++;
	}
	return (ft_strdup(cmd[0]));
}

// make full path of the bin file
char	*get_path(char **cmd, char **envp)
{
	char	*path;
	char	**temp;
	int		i;

	i = 0;
	if (!envp)
		return (ft_strdup(cmd[0]));
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	if (ft_strncmp(envp[i], "PATH=", 5))
		return (ft_strdup(cmd[0]));
	temp = ft_split(&envp[i][5], ':');
	if (!temp)
		return (perror("ft_split failed"), NULL);
	path = find_bin(cmd, temp, 0);
	ft_free_pipex(temp);
	if (!path)
		return (perror("ft_strdup failed"), NULL);
	return (path);
}

int	empty_pipe(int *fd)
{
	if (dup2(fd[1], 1) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("dup2 failed");
		return (1);
	}
	if (dup2(fd[0], 0) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("dup2 failed");
		return (1);
	}
	close(fd[0]);
	close(fd[1]);
	return (0);
}

// wait for each child to finish
// if it is a valid pipe then update exit code of each pipe
void	wait_pipes(t_token *token)
{
	int	i;
	int	status;

	i = 0;
	while (i < token->nmb_of_cmd && token->error[0] == 0)
	{
		if (token->cmds[i]->pid != 0)
		{
			waitpid(token->cmds[i]->pid, &status, 0);
			token->cmds[i]->exit_code = WEXITSTATUS(status);
		}
		//if (token->cmds[i]->fd[0] != -1)
		token->exit_code[0] = token->cmds[i]->exit_code;
		i++;
	}
}
