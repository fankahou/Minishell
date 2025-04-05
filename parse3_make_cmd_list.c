/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_make_cmd_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/05 14:42:30 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// split the temp cmd with a modified split to seperate the cmd and arg with space
// space inside a quote will not be skipped
// quotes and $ are still inside yet and will be cleaned and expanded later
// join the new split with previous split
// eg. "<infile echo      >out 'hel   lo' world$USER" will be
// echo
// 'hel   lo'
// world$USER
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

//temp is the new splited cmd list with redir syntax in between
//scan temp and parse info into struct
//redir if the array starts with a redir syntax
//ft_cmd if it is something else
//if it's only a valid syntax inside a pipe eg. ">out | echo 123", just return 0
//free and replace token->cmds[k]->cmd with the parsed **cmd
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
            cmd = ft_cmd(&temp[i][j], token, cmd);
            if (!cmd)
                return (token->error[0] = 1, 1);
        }
        if (j == -1)
            return (token->error[0] = 1, 1);
    }
    if (is_sym(token->cmds[k]->cmd[0][0], token->cmds[k]->cmd[0][1]) && cmd == NULL)
        return(ft_free_split(token->cmds[k]->cmd), token->cmds[k]->cmd = NULL, 0);
    ft_free_split(token->cmds[k]->cmd);
    token->cmds[k]->cmd = cmd;
    //print_array(cmd);
    return (0);
}

//make cmd list of all splited pipes
//will skip an array if the array is a pipe '|'
//split cmds by redir syntax < << >> >
//the syntax also takes one array
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

//make cmd list of all splited pipes
//split cmds by redir syntax < << >> >
//the syntax also takes one array
//scan all pipes
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
