/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/07 14:33:18 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//to do list 3 April
// mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD; unset OLDPWD; pwd
// cat /dev/random | head -c 100 | wc -c // waitpid ??? signal?
// ls '" and ls "' // just leave it as it is?

// echo hello $NOT_A_VAT $NOT_A_VAT $NOT_A_VAT// with space handling still one last space left
// export L="ls -la"; $L // change tree order and split?
// mkdir a a/b; cd a/b; rm -rf ../../a; pwd // also cd .. instead of pwd // and plus unset PWD OLDPWD in between
// unset HOME; cd $HOME // also unset PATH  // just leave it as it is?

/**
 * @brief Debug function to print tokens.
 * 
 * Debug funciton that prints the contents
 * of each token in an array of tokens.
 * 
 * @param token token array to print
 *
 * @author kfan
 */
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

/**
 * @brief Main function of the program
 *
 * This is the main function and entry point of the program.
 * As a wise man once said: "This is where the magic happens!"
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param envp Environment
 * @return int
 * @retval exit_code Exit code of the program.
 *
 * @author kfan
 */
int main(int argc, char **argv, char**envp)
{
    t_data data;
    t_history history;
    char *temp;

    signal_init();
    (void) argc;
    (void) argv;
    data.str = NULL;
    data.exit_code = 0;
    data.error = 0;
    data.fd[0] = 0;
    data.fd[1] = 1;
    //envp = NULL; // just to test
    data.envp = NULL;
    data.envp_export = NULL;
    if (envp && envp[0]) // new: protection!
    {
        data.envp = copy_array(envp);
        data.envp_export = copy_array_prefix(envp, 0 , 0, 0); // new!!!!!!!!!!!!!!!!!!!!!!!!!!!
        sort_array(data.envp_export); // new!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    init_history(&history);
    while (1)
    {
        temp = readline("minishell> "); // new: one more space just for better read
        printf("data.error = %d\n", data.error);
        if (temp && temp[0] == '\0')
        {
            free(temp);
            break ;
        }
        data.str = ft_calloc(ft_strlen(temp) + 2, 1); //new: copy one more \0 to imitate gnl to avoid invalid read in Ka Hou's code
        if (!data.str)
            break ;
        ft_strlcpy(data.str, temp, ft_strlen(temp) + 1);
        free(temp);
        debug(data.str);
        make_tree(&data); // from here Ka Hou's code begins
        history_add(&history, data.str); // new: Ka Hou changed it to after make_tree instead of before otherwise Koloman frees the cmd before it goes into make_tree
        data.str = NULL;
        if (data.error == 2)
            break;
    }
    if (data.str)
        free(data.str);
    if (data.fd[0] != 0) // make sure Koloman close!!!
        close(data.fd[0]);
    if (data.fd[1] != 1)
        close(data.fd[1]);
    ft_free_split(data.envp); //  make sure Koloman free!!!
    ft_free_split(data.envp_export); //  make sure Koloman free!!!
    destroy_history(&history);
    return (data.exit_code);
}
