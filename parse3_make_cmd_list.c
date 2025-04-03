/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_make_cmd_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 14:08:02 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// new split
char **ft_cmd(char *temp, t_token *token, char **old)
{
    char **cmd;

    cmd = ft_split_space(temp); // need custom split! need to check?
    if (!cmd)
        return (perror("ft_split failed"), token->error[0] = 1, NULL);
    if (old)
        old = join_split(&old, &cmd, 0, 0);
    else
        old = cmd;
    if (!old)
        return (token->error[0] = 1, NULL);
    return (old);
}


static int split_cmd(t_token *token, char **temp)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (temp[i])
    {
        if (!is_pipe(temp[i][0]))
        {
            token->cmds[j]->cmd = ft_split_cmd(temp[i]);
            if (!token->cmds[j]->cmd)
                return(token->error[0] = 1, perror("ft_split_cmd failed"), 1);
            j++;
        }
        i++;
    }
    return (0);
}

//new with cmd for split
static int scan_pipe(char **temp, t_token *token, int k, char **cmd)
{
    int i;
    int j;
    
    i = -1;
    while(temp[++i])
    {
        j = 0;
        while (is_space(temp[i][j]))
            j++;
        if (temp[i][j] && is_sym(temp[i][j], temp[i][j + 1]) > 2)
            j = redir(&temp[i][j], token, k);
        else if (temp[i][j])
        {
            cmd = ft_cmd(&temp[i][j], token, cmd);//, 0);
            if (!cmd)
                return (token->error[0] = 1, 1);
        }
        if (j == -1)
            return (token->error[0] = 1, 1);
    }
    if (is_sym(token->cmds[k]->cmd[0][0], token->cmds[k]->cmd[0][1]) && cmd == NULL) // need to check! && cmd = NULL?
        return(ft_free_split(token->cmds[k]->cmd), token->cmds[k]->cmd = NULL, 0);
    ft_free_split(token->cmds[k]->cmd);
    token->cmds[k]->cmd = cmd;
    //print_array(cmd);
    return (0);
}

int make_cmd_list(char **temp, t_token *token)
{
    int k;

    k = 0;
    if (split_cmd(token, temp))
        return(1);
    while (token->cmds[k])
    {
        if (scan_pipe(token->cmds[k]->cmd, token, k, NULL))
            return (1);
        k++;
    }
    return (0);
}
