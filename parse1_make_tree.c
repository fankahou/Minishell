/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_make_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/01 21:41:23 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo 123 >out1 | echo 456 >>out 2 | echo 789 >> out3 | >out4 | echo 678 ; <out1 cat | cat > out; << EOF cat | cat >> out4
// echo 123 >out1 | echo 456 >>out 2 | echo 789 >> out3 | >out4 | echo 678 ; <out1 cat | cat > out; << EOF cat | cat >> out4
/**
 * @brief I have no idea what this does.
 * 
 * Please write some Documentation here, Ka Hou. 
 *
 * @param temp (description of temp)
 * @param error_count (description of error_count)
 * @return int 
 * @retval k (description of k)
 */
static int check_temp(char **temp, int error_count)
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
        if (temp[i][j] == '\0')
            k++;
        else if (is_delimiter(temp[i][j], temp[i][j + 1]))
            error_count++;
        else
        {
            error_count--;
            k++;
        }
        if (error_count > 0)
            return (syntax_error(temp[k], NULL), 0);
        i++;
    }
    return (k);
}

/**
 * @brief Creates a token.
 * 
 * Creates an array of token structs and
 * initialises them with the commands inside
 * temp and the data inside data
 *
 * @param temp command list
 * @param token Array of tokens
 * @param data data struct pointer
 * @return t_token** 
 * @retval token new token or NULL on error
 *
 * @ref t_token
 * @ref t_data
 *
 * @author kfan
 */
static t_token **make_token(char **temp, t_token **token, t_data *data)
{
    int i;
    
    i = check_temp(temp, 0);
    if (i == 0)
        return (data->exit_code = 2, NULL);
    token = malloc(sizeof(t_token *) * (i + 1));
    if (!token)
        return (perror("malloc failed\n"), NULL);
    token[i] = NULL;
    if (init_token(temp, token, data, i))
        data->error = 1;
    return (token);
}

// update envp after each pipex
/**
 * @brief Executes tokens.
 * 
 * This function executes a list of tokens. This function
 * is also a breaking point for execution, should something
 * go wrong inside it.
 *
 * @param token Array of tokens to execute
 * @param data Data struct containing needed information
 * @param fd Array od duplicated file descriptors
 * @return int 
 * @retval success always returns 0
 *
 * @ref t_token
 * @ref t_data
 *
 * @author kfan
 */
static int execute(t_token **token, t_data *data, int *fd)
{
    int i;

    i = 0;
    while(token[i])
    {
        token[i]->envp = data->envp;
        token[i]->envp_export = data->envp_export;
        if (clean_and_expand(token[i]))
            return (data->error = 1, 1);
        if (token[i]->nmb_of_cmd > 0)
            pipex(token[i]);
        if (token[i]->delimiter == 2 && token[i]->exit_code[0] != 0)
            break;
        if (token[i]->delimiter == 3 && token[i]->exit_code[0] == 0)
            break;
        if (data->error != 0)
            break;
        restore_fd(data, fd); // reset after each pipe???
        i++;
    }
    //update envp "echo $_"???
    return (0);
}

/**
 * @brief Initializes the data for token processing.
 *
 * This function initializes the tree creation process.
 * It duplicates STDIN and STDOUT using dup() and writes
 * them into fd[0] and fd[1] respectively.
 * 
 * @param data pointer to data struct
 * @param fd pointer to duplicate file descriptors into
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan 
 */
static int init_tree(t_data *data, int *fd)
{
    fd[0] = dup(data->fd[0]);
    if (fd[0] == -1)
        return(perror("dup failed"), 1);
    fd[1] = dup(data->fd[1]);
    if (fd[1] == -1)
        return(close(fd[0]), perror("dup failed"), 1);
    data->fd_in = fd[0];
    data->fd_out = fd[1];
    data->error = 0; // fresh restart
    return (0);
}

// store STDIN and STDOUT for recovery after each pipex
// error != 0 will not excute
// split input by delimiters ; && ||
// make array of struct token
/**
 * @brief Creates the tree of tokens to be processed.
 *
 * Splits the input string into an array of tokens
 * to be processed and starts the execution.
 * The string is split on the following delimiters:
 * - ;
 * - &&
 * - ||
 * STDIN and STDOUT are cached for easy recovery after each
 * call to pipex.
 * If data->error is not 0, it will not execute the token. 
 * 
 * @param data 
 *
 * @ref t_token token struct
 * @ref pipex pipex function
 * 
 * @author kfan
 */
void make_tree(t_data *data)
{
    char **temp;
    t_token **token;
    int fd[2]; // for restoring STD_IN and STD_OUT
    
    if (init_tree(data, fd))
        return ;
    token = NULL;
    temp = ft_split_delimiter(data->str);
    if (!temp)
        perror("ft_split_delimiter failed\n");
    else
    {
        token = make_token(temp, token, data);
        ft_free_split(temp);
        //print_token(token);
        if(token && data->error == 0)
            execute(token, data, fd);
        if (token)
            close_unused_fd(token, 0);
        if (token)
            ft_free_token(token);
    }
    restore_fd(data, fd);
    close(fd[0]);
    close(fd[1]);
}
