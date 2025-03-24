/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 11:47:47 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_split(char **array)
{
    int i;
    
    if (!array)
        return ;
    i = 0;
    while (array[i])
        i++;
    while (i >= 0)
	{
        if (array[i])
		    free(array[i]);
		array[i] = NULL;
		i--;
	}
    if (array)
	    free(array);
	array = NULL;
}

static void    ft_free_struct(t_cmds **cmds, int n)
{
    int i;

    if (!cmds)
        return ;
    i = 0;
    while (i < n && cmds[i])
    {
        if (cmds[i]->cmd)
            ft_free_split(cmds[i]->cmd);
        if (cmds[i]->infile)
            free(cmds[i]->infile); // close?
        if (cmds[i]->outfile)
            free(cmds[i]->outfile);
        free(cmds[i]);
        i++;
    }
    free(cmds);
    cmds = NULL;
}

void	ft_free_token(t_token **token)
{
    int i;
    
    if (!token)
        return ;
    i = 0;
    while (token[i])
	{
        if (token[i])
        {
            ft_free_struct(token[i]->cmds, token[i]->nmb_of_cmd);
		    free(token[i]);
        }
		token[i] = NULL;
		i++;
	}
    if (token)
	    free(token);
	token = NULL;
}

// used only if malloc failed in init_token()
void    small_free(t_token **token, int i)
{
    int j;

    j = i;
    while (j > 0)
        token[--j] = NULL;
    i++;
    while (token[i])
    {
        free(token[i]);
        token[i] = NULL;
        i++;
    }
}
