/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/09 16:15:24 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pipes for builtins
int	builtins_pipe_fd_out(t_cmds *cmds, int *fd)
{
	if (fd && (str_equals(cmds->cmd[0], "echo")
			|| str_equals(cmds->cmd[0], "cd")
			|| str_equals(cmds->cmd[0], "pwd")
			|| str_equals(cmds->cmd[0], "export")
			|| str_equals(cmds->cmd[0], "unset")
			|| str_equals(cmds->cmd[0], "env")
			|| str_equals(cmds->cmd[0], "exit")))
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
	if (fd && (str_equals(cmds->cmd[0], "echo")
			|| str_equals(cmds->cmd[0], "cd")
			|| str_equals(cmds->cmd[0], "pwd")
			|| str_equals(cmds->cmd[0], "export")
			|| str_equals(cmds->cmd[0], "unset")
			|| str_equals(cmds->cmd[0], "env")
			|| str_equals(cmds->cmd[0], "exit")))
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
