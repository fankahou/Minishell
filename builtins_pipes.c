/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/09 14:24:59 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pipes for builtins
int	builtins_pipe_fd_out(t_cmds *cmds, int *fd)
{
	if (fd && ((!ft_strncmp(cmds->cmd[0], "echo", 4)
				&& ft_strlen(cmds->cmd[0]) == 4) || (!ft_strncmp(cmds->cmd[0],
					"cd", 2) && ft_strlen(cmds->cmd[0]) == 2)
			|| (!ft_strncmp(cmds->cmd[0], "pwd", 3)
				&& ft_strlen(cmds->cmd[0]) == 3) || (!ft_strncmp(cmds->cmd[0],
					"export", 6) && ft_strlen(cmds->cmd[0]) == 6)
			|| (!ft_strncmp(cmds->cmd[0], "unset", 5)
				&& ft_strlen(cmds->cmd[0]) == 5) || (!ft_strncmp(cmds->cmd[0],
					"env", 3) && ft_strlen(cmds->cmd[0]) == 3)
			|| (!ft_strncmp(cmds->cmd[0], "exit", 4)
				&& ft_strlen(cmds->cmd[0]) == 4)))
	{
		if (!cmds->outfile && dup2(fd[1], 1) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2 failed");
			return (1);
		}
	}
	return (0);
}

int	builtins_pipe_fd_in(t_cmds *cmds, int *fd)
{
	if (fd && ((!ft_strncmp(cmds->cmd[0], "echo", 4)
				&& ft_strlen(cmds->cmd[0]) == 4) || (!ft_strncmp(cmds->cmd[0],
					"cd", 2) && ft_strlen(cmds->cmd[0]) == 2)
			|| (!ft_strncmp(cmds->cmd[0], "pwd", 3)
				&& ft_strlen(cmds->cmd[0]) == 3) || (!ft_strncmp(cmds->cmd[0],
					"export", 6) && ft_strlen(cmds->cmd[0]) == 6)
			|| (!ft_strncmp(cmds->cmd[0], "unset", 5)
				&& ft_strlen(cmds->cmd[0]) == 5) || (!ft_strncmp(cmds->cmd[0],
					"env", 3) && ft_strlen(cmds->cmd[0]) == 3)
			|| (!ft_strncmp(cmds->cmd[0], "exit", 4)
				&& ft_strlen(cmds->cmd[0]) == 4)))
	{
		if (dup2(fd[0], 0) == -1)
		{
			close(fd[0]);
			close(fd[1]);
			perror("dup2 failed");
			return (1);
		}
		close(fd[0]);
		close(fd[1]);
	}
	return (0);
}
