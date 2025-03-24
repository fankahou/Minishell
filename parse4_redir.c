/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/23 17:31:45 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void write_heredoc(t_token *token, char *temp, int fd, int k)
{
	int	n;
	
	n = ft_strlen(token->cmds[k]->infile);
	while (1)
	{
		write (token->fd_in, "heredoc> ", 9);
		temp = get_next_line(token->fd_in);
		if (!ft_strncmp(token->cmds[k]->infile, temp, n))
		{
			if (temp)
				free(temp);
			get_next_line(-1);
			break ;
		}
		write(fd, temp, ft_strlen(temp));
		if (temp)
			free(temp);
	}
}

static int	ft_heredoc(t_token *token, int k)
{
	int		fd;
	char	*temp;

	temp = NULL;
	if (access("temp", F_OK) == 0)
		unlink("temp");
	fd = open("temp", O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (-1);
	write_heredoc(token, temp, fd, k);
	close(fd);
	fd = open("temp", O_RDONLY);
	if (fd < 0)
		return (-1);
	if (access("temp", F_OK) == 0)
		unlink("temp");
	return (fd);
}


static int ft_redir_in(t_token *token, int type, char *file, int k)
{
    //int fd;
    
    if (token->cmds[k]->redir[0] > 0 && token->cmds[k]->infile)
    {
        if (token->cmds[k]->redir[0] == 3 && token->cmds[k]->fd[0] != token->data->fd[0])
            close(token->cmds[k]->fd[0]);
        free(token->cmds[k]->infile);
    }
    token->cmds[k]->infile = file;
    if (type == 3)
    {
        token->cmds[k]->redir[0] = 3;
        token->cmds[k]->fd[0] = ft_heredoc(token, k);
        if (token->cmds[k]->fd[0] == -1)
            return(perror("heredoc failed"), -1);
    }
    else
        token->cmds[k]->redir[0] = 4;
    return (0);
}

static void ft_redir_out(t_token *token, int type, char *file, int k)
{
    if (token->cmds[k]->redir[1] > 0 && token->cmds[k]->outfile)
        free(token->cmds[k]->outfile);
    token->cmds[k]->outfile = file;
    token->cmds[k]->redir[1] = 5;
    if (type == 6)
        token->cmds[k]->redir[1] = 6;
}

int redir(char *temp, t_token *token, int k)
{
    char *file;
    int return_val;
    int type;
    int count;

    return_val = 0;
    type = is_sym(temp[0], temp[1]);
    count = sym_count(temp[0], temp[1], NULL);
    file = ft_calloc(1, 1);
    if (!file)
        return (ft_printf("ft_calloc failed\n"), -1);
    file = clean_name(temp, token, count, file);
    if (!file)
        return (syntax_error(&temp[0], token), -1);
    if (type == 3 || type == 4)
        return_val = ft_redir_in(token, type, file, k);
    else if (type == 5 || type == 6)
        ft_redir_out(token, type, file, k);
    return (return_val);
}
