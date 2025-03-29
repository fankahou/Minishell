/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/03/29 13:43:13 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pipex(char **array)
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
/* 
void	error_pipex(char *message, char **temp)
{
	perror(message);
	if (temp)
		ft_free_pipex(temp);
} */

char	*find_bin(char **cmd, char **paths, int i)
{
	char	*temp;
	char	*temp1;

	while (paths[i] && ft_strncmp(cmd[0], "./", 2))
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (perror("strjoin failed"), NULL);
		temp1 = ft_strjoin(temp, cmd[0]); //
		free(temp);
		if (!temp1)
			return (perror("strjoin failed"), NULL);
		if (access(temp1, F_OK) == 0)
			return (temp1);
		free(temp1);
		i++;
	}
	//printf("test1\n");
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
