/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_clean_and_expand.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 18:37:21 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int join_cmd_1(t_token *token, int i, int j, char *file)
{
    int k;
    char **temp;
    
    //print_array(token->data->cmd_temp);
    //printf("file = %s\n", file);
    free(file);
    temp = NULL;
    k = 0;
    while (token->data->cmd_temp[k])
        k++;
    temp = copy_array(&token->cmds[i]->cmd[j + 1]);
    while (token->cmds[i]->cmd[++j])
        free(token->cmds[i]->cmd[j]);
    if (!temp)
        return (perror("copy_array failed"), token->error[0] = 1, 0);;
    token->data->cmd_temp = join_split(&token->data->cmd_temp, &temp, 0, 0);
    token->cmds[i]->cmd = join_split(&token->cmds[i]->cmd, &token->data->cmd_temp, 0, 0); // protection??
    return (k - 1);
}

// skip a cmd if !file or copy it 
static int join_cmd_2(t_token *token, int i, int j, char *file)
{
    char **temp;
    
    ft_free_split(token->data->cmd_temp);
    if (file && file[0] != '\0')
    {
        token->cmds[i]->cmd[j] = file;
        return (0);
    }
    free(file);
    temp = copy_array(&token->cmds[i]->cmd[j + 1]);
    while (token->cmds[i]->cmd[++j])
        free(token->cmds[i]->cmd[j]);
    token->data->cmd_temp = NULL;
    if (!temp)
        return (perror("copy_array failed"), token->error[0] = 1, 0);
    token->cmds[i]->cmd = join_split(&token->cmds[i]->cmd, &temp, 0, 0); // protection??
    return (-1);
}

static int clean_cmd(t_token *token)
{
    char *file;
    int i;
    int j;
    
    i = 0;
    while (token->cmds && token->cmds[i])
    {
        j = 0;
        while (token->cmds[i]->cmd && token->cmds[i]->cmd[j])
        {
            file = ft_calloc(1, 1);
            if (!file)
                return (ft_free_split(token->cmds[i]->cmd), perror("ft_calloc failed"), token->error[0] = 1, 1);
            file = clean_name(token->cmds[i]->cmd[j], token, 0, file);
            if (!file)
                return (ft_free_split(token->cmds[i]->cmd), token->error[0] = 1, 1);
            free(token->cmds[i]->cmd[j]);
            token->cmds[i]->cmd[j] = NULL;
            //print_array(token->data->cmd_temp);
            //printf("file = %s\n", file);
            if (token->data->cmd_temp && token->data->cmd_temp[0] && token->data->cmd_temp[0][0])
                j = j + join_cmd_1(token, i, j, file);
            else if (token->data->cmd_temp)
                j = j + join_cmd_2(token, i, j, file);
            else
                token->cmds[i]->cmd[j] = file;
            j++;
            if (token->error[0] != 0)
                return (1);
        }
        i++;
    }
    return (0);
}

static int clean_outfile(t_token *token)
{
    char *file;
    int i;
    
    i = 0;
    while (token->cmds && token->cmds[i])
    {
        if (token->cmds[i]->outfile)
        {
            file = ft_calloc(1, 1);
            if (!file)
                return (ft_printf("ft_calloc failed\n"), 1);
            file = clean_name(token->cmds[i]->outfile, token, 0, file);
            if (!file)
                return (syntax_error(token->cmds[i]->outfile, token), 1);
            if (file[0] == '\0')
                return (free(file), mini_error("ambiguous redirect", token->cmds[i]->outfile, token, token->cmds[i]->fd), 0);
            free(token->cmds[i]->outfile);
            token->cmds[i]->outfile = file;
        }
        i++;
    }
    return (0);
}

static int clean_infile(t_token *token)
{
    char *file;
    int i;
    
    i = 0;
    while (token->cmds && token->cmds[i])
    {
        if (token->cmds[i]->infile)
        {
            file = ft_calloc(1, 1);
            if (!file)
                return (ft_printf("ft_calloc failed\n"), 1);
            file = clean_name(token->cmds[i]->infile, token, 0, file);
            if (!file)
                return (syntax_error(token->cmds[i]->infile, token), 1);
            if (file[0] == '\0')
                return (free(file), mini_error("ambiguous redirect", token->cmds[i]->infile, token, token->cmds[i]->fd), 0);
            free(token->cmds[i]->infile);
            token->cmds[i]->infile = file;
        }
        i++;
    }
    return (0);
}

int clean_and_expand(t_token *token)
{
    if (clean_outfile(token))
        return (1);
    if (clean_infile(token))
        return (1);
    if (clean_cmd(token))
        return (1);
    return (0);
}
