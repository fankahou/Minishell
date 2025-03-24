/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_make_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 13:09:18 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo 123 >out1 | echo 456 >>out 2 | echo 789 >> out3 | >out4 | echo 678 ; <out1 cat | cat > out; << EOF cat | cat >> out4
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

static int execute(t_token **token, t_data *data, int *fd)
{
    int i;

    i = 0;
    while(token[i])
    {
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
    return (0);
}

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
    data->error = 0;
    return (0);
}

// store STDIN and STDOUT for recovery after each pipex
// error != 0 will not excute
// split input by delimiters ; && ||
// make array of struct token
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
