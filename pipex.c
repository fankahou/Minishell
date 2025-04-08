/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/08 13:25:20 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// token->redir[1] == 6 == Append mode
// otherwise will create new
int	outfile(t_token *token, int k)
{
	int	fd;

	if (token->cmds[k]->fd[1] == -1)
		return (-1);
	if (access(token->cmds[k]->outfile, F_OK) == 0)
	{
		if (access(token->cmds[k]->outfile, W_OK) != 0)
			return (perror("minishell: Permission denied"),
					token->exit_code[0] = 13, -1); // exitcode = 13???
	}
	if (token->cmds[k]->redir[1] == 6)
		fd = open(token->cmds[k]->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	else
	{
		if (access(token->cmds[k]->outfile, F_OK) == 0)
			unlink(token->cmds[k]->outfile);
		fd = open(token->cmds[k]->outfile, O_WRONLY | O_CREAT, 0664);
	}
	if (fd < 0)
		open_error(token->cmds[k]->outfile, token, token->cmds[k]->fd);
	return (fd);
}

static int	infile(t_token *token, int k)
{
	int	fd_in;

	if (token->cmds[k]->redir[0] == 3)
		return (token->cmds[k]->fd[0]);
	else
	{
		/* 		if (access(token->cmds[k]->infile, F_OK) == 0
					&& access(token->cmds[k]->infile, R_OK) != 0)
				{
					perror("minishell: Permission denied\n");
					token->exit_code[0] = 13; // test it?
					token->error[0] = 1;
					return(-1);
				} */
		fd_in = open(token->cmds[k]->infile, O_RDONLY);
	}
	if (fd_in < 0)
		open_error(token->cmds[k]->infile, token, token->cmds[k]->fd);
	return (fd_in);
}

static int	open_fd(t_token *token, int i)
{
	if (token->cmds[i]->fd[1] != -1 && token->cmds[i]->outfile)
	{
		token->cmds[i]->fd[1] = outfile(token, i);
		/* 		if (token->cmds[i]->fd[0] == -1)
					return(1); */
	}
	if (token->cmds[i]->fd[1] != -1 && token->cmds[i]->infile)
	{
		token->cmds[i]->fd[0] = infile(token, i);
		/* 		if (token->cmds[i]->fd[0] == -1)
					return(1); */
	}
	return (0);
}

static int	replace_fd(t_token *token, int i)
{
	if (token->cmds[i]->infile) // replace fd
	{
		if (token->cmds[i]->fd[0] != -1 && dup2(token->cmds[i]->fd[0], 0) == -1)
			return (close(token->cmds[i]->fd[0]), 1);
		if (token->cmds[i]->fd[0] != -1)
			close(token->cmds[i]->fd[0]);
	}
	else if (i > 0 && token->cmds[i - 1]->outfile
		&& dup2(token->fd_in, 0) == -1)
		// restore st_in to 0 again if the pipe was already in previous outfile
		return (1);
	else if (i > 0 && i != token->nmb_of_cmd && dup2(token->cmds[i - 1]->fd[1],
			0) == -1) // !!!new: pipes in between
		return (1);
	if (token->cmds[i]->outfile)
	{
		if (token->cmds[i]->fd[1] != -1 && dup2(token->cmds[i]->fd[1], 1) == -1)
			return (close(token->cmds[i]->fd[1]), 1);
		if (token->cmds[i]->fd[1] != -1)
			close(token->cmds[i]->fd[1]);
	}
	else if (i == token->nmb_of_cmd && dup2(token->fd_out, 1) == -1)
		// copy LAST st_out to 1 again (i == token->nmb_of_cmd  is new!!!)
		return (1);
	return (0);
}

int	pipex(t_token *token)
{
	int	i;
	int	status;

	i = 0;
	while (i < token->nmb_of_cmd - 1 && token->error[0] == 0)
	{
		open_fd(token, i);
		if (replace_fd(token, i))
			return (perror("dup2 failed"), 1);
		if (token->cmds[i]->cmd && input(token->cmds[i], token))
			return (1);
		i++;
	}
	if (token->cmds[i] && token->error[0] == 0)
	{
		open_fd(token, i);
		if (replace_fd(token, i))
			return (perror("dup2 failed"), 1);
		if (token->cmds[i]->fd[0] != -1 && token->cmds[i]->cmd
			&& last_input(token->cmds[i], token))
			return (1);
	}
	i = 0;
	while (i < token->nmb_of_cmd && token->error[0] == 0) // new wait!!!
	{
		if (token->cmds[i]->pid != 0)
			waitpid(token->cmds[i]->pid, &status, 0);
		// printf("pid = %d\n", token->cmds[i]->pid);
		i++;
	}
	return (0);
}
