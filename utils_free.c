/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/04/07 13:33:32 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Frees the result of an ft_split() call.
 *
 * Frees a 2-dimensional array like one returned by ft_split().
 * The array has to be NULL-terminated!
 * 
 * @param array array to free
 * 
 * @author kfan
 */
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

/**
 * @brief Frees a command list
 *
 * Frees a t_cmds command list struct.
 * 
 * @param cmds t_cmds struct to free
 * @param n number of commands in the list
 *
 * @ref t_cmds
 *
 * @author kfan
 */
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

/**
 * @brief Frees a token.
 *
 * Frees a t_token struct.
 * 
 * @param token t_token struct to free
 *
 * @ref t_token
 *
 * @author kfan
 */
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
/**
 * @brief Small version of ft_free_token()
 * 
 * Call this instead of ft_free_token() when
 * the malloc() inside ft_init_token() fails!
 *
 * @param token token to free
 * @param i size of the token data
 *
 * @ref t_token
 * @ref init_token
 * @ref ft_free_token
 *
 * @author kfan
 */
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
