/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/31 14:25:18 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_cmd(char **temp, t_token *token, int error_count)
{
    int i;
    int j;
    int k;
    
    i = 0;
    k = 0;
    while (temp[i])
    {
        j = 0;
        while (is_space(temp[i][j]))
            j++;
        if (is_pipe(temp[i][j]))
            error_count++;
        else
        {
            error_count = 0;
            k++;
        }
        if (error_count == 2)
            return (syntax_error("|", token), -1);
        i++;
    }
    if (i > 0 && is_pipe(temp[i - 1][j]))// && token->delimiter > 0)
        return(syntax_error("|", token), -1);
    return (k);
}

static int check_and_malloc(char **temp, t_token *token)
{
    int i;
    
    i = 0;
    token->nmb_of_cmd = check_cmd(temp, token, 1);
    if (token->nmb_of_cmd == -1)
        return(1);
    token->cmds = malloc(sizeof(t_cmds *) * (token->nmb_of_cmd + 1));
    if (!token->cmds)
        return (token->error[0] = 1, perror("malloc failed\n"), 1);
    while (i < token->nmb_of_cmd)
    {
        token->cmds[i] = malloc(sizeof(t_cmds));
        if (!token->cmds[i])
            return(token->error[0] = 1, perror("malloc failed"), 1);
        token->cmds[i]->cmd = NULL;
        token->cmds[i]->redir[0] = 0;
        token->cmds[i]->redir[1] = 0;
        token->cmds[i]->infile = NULL;
        token->cmds[i]->outfile = NULL;
        token->cmds[i]->fd[0] = token->data->fd[0];
        token->cmds[i]->fd[1] = token->data->fd[1];
        i++;
    }
    token->cmds[i] = NULL;
    return (0);
}

static int split_pipe(char *cmds, t_token *token)
{
    char **temp;
    
    temp = ft_split_pipe(cmds);
    if (!temp)
        return (perror("ft_split_pipe failed"), token->error[0] = 1, 1);
    if (check_and_malloc(temp, token))
        return (ft_free_split(temp), 1);
    make_cmd_list(temp, token); //protection?
    ft_free_split(temp);
    if (!token->cmds)
        return (token->error[0] = 1, 1);
    return (0);
}

/**
 * @brief Second part of token initialisation
 *
 * This function is a mystery to me -_-
 * 
 * @param temp command list
 * @param token token array
 * 
 * @ref t_token
 *
 * @author kfan
 */
static void init_token1(char **temp, t_token **token)
{
    int i;
    int j;
    int k;
    
    i = 0;
    k = 0;
    while (temp[i])
    {
        if (temp[i + 1])
        {
            j = 0;
            while (is_space(temp[i + 1][j]))
                j++;
            token[k]->delimiter = is_delimiter(temp[i + 1][j], temp[i + 1][j]);
        }
        if (split_pipe(temp[i], token[k]))
            return ;
        i++;
        if (!temp[i])
            break ;
        else
            i++;
        k++;
    }
    //print_token(token);
}

/**
 * @brief Initialises a token array.
 * 
 * Sets the parameters taken from data
 * in every token of a token array.
 * 
 * @param temp command list
 * @param token array of t_token structs
 * @param data t_data struct
 * @param i length of the token array
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 *
 * @ref t_token
 * @ref t_data
 *
 * @author kfan
 */
int init_token(char **temp, t_token **token, t_data *data, int i)
{
    while (i > 0)
    {
        i--;
        token[i] = malloc(sizeof(t_token));
        if (!token[i])
            return (small_free(token, i), perror("malloc failed"), 1);
        token[i]->cmds = NULL;
        //check if !envp?
        token[i]->envp = data->envp;
        token[i]->envp_export = data->envp_export;
        token[i]->exit_code = &data->exit_code;
        token[i]->fd_in = data->fd_in;
        token[i]->fd_out = data->fd_out;
        token[i]->error = &data->error;
        token[i]->delimiter = 0;
        token[i]->nmb_of_cmd = 0;
        token[i]->data = data;
    }
    init_token1(temp, token);
    return (0);
}
