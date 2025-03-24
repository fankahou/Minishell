/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 15:23:50 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// for debugging 
void print_array(char **temp)
{
    int i;

    i = 0;
    while (temp[i])
    {
        ft_printf("%s\n", temp[i]);
        i++;
    }
}

// for debugging 
void print_token(t_token **token)
{
    int i;
    int j;

    i = 0;
    if (!token)
        return ;
    while (token[i])
    {
        j = 0;
        if (token[i]->cmds)
        {
            while(token[i]->cmds[j])
            {
                if (token[i]->cmds[j]->cmd)
                    print_array(token[i]->cmds[j]->cmd);
                else
                    printf("no cmd\n");
/*                 k = 0;
                while(token[i]->cmds[j]->cmd[k])
                    ft_printf("%d: cmd[%d][%d] = %s$\n", i, j, k, token[i]->cmds[j]->cmd[k++]); */
                ft_printf("%d: redir[0] (in) = %d\n", i, token[i]->cmds[j]->redir[0]);
                ft_printf("%d: redir[1] (out) = %d\n", i, token[i]->cmds[j]->redir[1]);
                ft_printf("%d: fd[0] (in) = %d\n", i, token[i]->cmds[j]->fd[0]);
                ft_printf("%d: fd[1] (out) = %d\n", i, token[i]->cmds[j]->fd[1]);
                ft_printf("%d: infile = %s$\n", i, token[i]->cmds[j]->infile);
                ft_printf("%d: outfile = %s$\n\n", i, token[i]->cmds[j]->outfile);
                j++;
            }
        }
        //ft_printf("%d: pipes = %d\n", i, token[i]->delimiter);
        ft_printf("%d: delimiter = %d\n", i, token[i]->delimiter);
        ft_printf("%d: nmb_of_cmd = %d\n", i, token[i]->nmb_of_cmd);
        ft_printf("%d: exit_code = %d\n", i, token[i]->exit_code[0]);
        i++;
    }
}

char	**copy_array(char **input)
{
	char	**temp;
	int		y;
    int     i;

    y = 0;
    while (input[y])
        y++;
    temp = malloc(sizeof(char *) * (y + 1));
	if (!temp)
		return (perror("malloc failed"), NULL);
    temp[y] = NULL;
	i = 0;
	while (i < y)
	{
		temp[i] = ft_strdup(input[i]);
		if (!temp[i])
        {
            while (i > 0)
                free(input[--i]);
            return (perror("ft_strdup failed"), NULL);
        }
		i++;
	}
	return (temp);
}

int main(int argc, char **argv, char**envp)
{
    int i;
    t_data data;

    (void) argc;
    (void) argv;
    data.exit_code = 0;
    data.fd[0] = 0;
    data.fd[1] = 1;
    data.envp = copy_array(envp);
    write(data.fd[1], "minishell> ", 11);
    while(1)
    {
        i = 0;
        data.str = get_next_line(data.fd[0]); // fd 0 will not be STD_IN anymore!
        if (!data.str)
            continue ;
        if (data.str[0] == '\n')
            break ;
        while (data.str[i] != '\n')
            i++;
        data.str[i] = '\0';
        make_tree(&data); //from here Ka Hou's code begins
        free(data.str); //to be freeed by Koloman?
        data.str = NULL;
        get_next_line(-1);
        wait(0);
        write(data.fd[1], "minishell> ", 11); // use write instead of printf? otherwise all printf afterwards will end in wrong fd?
    }
    if (data.fd[0] != 0) // make sure Koloman close!!!
        close(data.fd[0]);
    if (data.fd[1] != 1)
        close(data.fd[1]);
    if (data.str)
        free(data.str);
    ft_free_split(data.envp); //  make sure Koloman free!!!
    get_next_line(-1);
    return (data.exit_code);
}
