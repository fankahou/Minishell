/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_make_cmd_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 12:45:23 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return (1) for skipping the $ sign
static int ft_cmd(char *temp, t_token *token, int k, int count)
{
    char *file;

    file = ft_calloc(1, 1);
    if (!file)
        return (perror("ft_calloc failed"), token->error[0] = 1, -1);
    file = clean_name(temp, token, 0, file);
    if (!file)
        return (token->error[0] = 1, -1);
    //printf("file = %s---\n", file);
    if (count > 0)
        token->cmds[k]->cmd[0] = join_cmd(token->cmds[k]->cmd[0], file);
    else
    {
        if (token->cmds[k]->cmd[0])
            free(token->cmds[k]->cmd[0]);
        token->cmds[k]->cmd[0] = file;
    }
    //if (!token->cmds[k]->cmd[0])
    return (1);
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

static char **replace_cmd(char **temp)
{
    char **new;
    int i;

    new = malloc(sizeof(char *) * 3);
    if (!new)
        return (perror("malloc failed"), ft_free_split(temp), NULL);
    new[1] = NULL;
    new[2] = NULL;
    i = 0;
    while (temp[0][i] && temp[0][i] != ' ')
        i++;
    new[0] = ft_substr(temp[0], 0, i);
    if (!new[0])
        return (free(new), perror("ft_substr failed"), ft_free_split(temp), NULL);
    if (temp[0][i] == ' ')
    {
        new[1] = ft_substr(temp[0], i + 1, ft_strlen(&temp[0][i + 1]));
        if (!new[1])
            return(free(new[0]), free(new), perror("ft_substr failed"), ft_free_split(temp), NULL);
    }
    ft_free_split(temp);
    return (new);
}

static int scan_pipe(char **temp, t_token *token, int k)
{
    int i;
    int j;
    int count;
    
    i = 0;
    count = 0;
    while(temp[i])
    {
        j = 0;
        while (is_space(temp[i][j]))
            j++;
        if (temp[i][j] && is_sym(temp[i][j], temp[i][j + 1]) > 2)
            j = redir(&temp[i][j], token, k);
        else if (temp[i][j])
            count = ft_cmd(&temp[i][j], token, k, count);
        if (j == -1)
        return (token->error[0] = 1, 1);
        //return (ft_free_split(token->cmds[k]->cmd), token->nmb_of_cmd = j, 1);
        i++;
    }
    if (is_sym(token->cmds[k]->cmd[0][0], token->cmds[k]->cmd[0][1]))
        return(ft_free_split(token->cmds[k]->cmd), token->cmds[k]->cmd = NULL, 0);
    token->cmds[k]->cmd = replace_cmd(token->cmds[k]->cmd);
    if (!token->cmds[k]->cmd)
        return (token->error[0] = 1, 1);
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
        if (scan_pipe(token->cmds[k]->cmd, token, k))
            return (1);
        k++;
    }
    return (0);
}
