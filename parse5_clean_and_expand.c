/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_clean_and_expand.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/28 18:05:46 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
            token->cmds[i]->cmd[j] = file;
            j++;
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
                return (syntax_error(token->cmds[i]->outfile, token), 1);
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
                return (syntax_error(token->cmds[i]->infile, token), 1); ///$FAKE: ambiguous redirect
            free(token->cmds[i]->infile);
            token->cmds[i]->infile = file;
        }
        i++;
    }
    return (0);
}

int clean_and_expand(t_token *token)
{
    if (clean_infile(token))
        return (1);
    if (clean_outfile(token))
        return (1);
    if (clean_cmd(token))
        return (1);
    return (0);
}
