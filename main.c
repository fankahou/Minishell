/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endermenskill <endermenskill@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/17 15:51:18 by endermenski      ###   ########.fr       */
=======
/*   Updated: 2025/04/17 15:12:23 by kfan             ###   ########.fr       */
>>>>>>> 8a3a6f9b8ac63eefcf2dad5f4f25f1f43f009173
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//to do list 7 April
//ctrl + c then <<eof cat and ctrl + c/ or d then ctrl + c, why extra \n????

//to do list 3 April
// mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD; unset OLDPWD; pwd
// cat /dev/random | head -c 100 | wc -c // waitpid ??? signal?
// ls '" and ls "' // just leave it as it is?

// echo hello $NOT_A_VAT $NOT_A_VAT $NOT_A_VAT
// with space handling still one last space left
// export L="ls -la"; $L // change tree order and split?
// mkdir a a/b; cd a/b; rm -rf ../../a; pwd // also cd .. instead of pwd
// and plus unset PWD OLDPWD in between
// unset HOME; cd $HOME // also unset PATH  // just leave it as it is?

/**
 * @brief Debug function to print tokens.
 * 
 * Debug funciton that prints the contents
 * of each token in an array of tokens.
 *
 * @author kfan
 * 
 * @param token token array to print
 */
/* void print_token(t_token **token)
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
                ft_printf("%d: redir[0] (in) = %d\n", i,
		token[i]->cmds[j]->redir[0]);
                ft_printf("%d: redir[1] (out) = %d\n", i,
		token[i]->cmds[j]->redir[1]);
                ft_printf("%d: fd[0] (in) = %d\n", i, token[i]->cmds[j]->fd[0]);
                ft_printf("%d: fd[1] (out) = %d\n", i,
		token[i]->cmds[j]->fd[1]);
                ft_printf("%d: infile = %s$\n", i, token[i]->cmds[j]->infile);
                ft_printf("%d: outfile = %s$\n\n", i,
		token[i]->cmds[j]->outfile);
                j++;
            }
        }
        ft_printf("%d: delimiter = %d\n", i, token[i]->delimiter);
        ft_printf("%d: nmb_of_cmd = %d\n", i, token[i]->nmb_of_cmd);
        ft_printf("%d: exit_code = %d\n", i, token[i]->exit_code[0]);
        i++;
    }
} */
static int	init_main(t_data *data, char **envp, t_history *history)
{
	data->str = NULL;
	data->exit_code = 0;
	data->error = 0;
	data->fd[0] = 0;
	data->fd[1] = 1;
	data->envp = NULL;
	data->envp_export = NULL;
	if (envp && envp[0])
	{
		data->envp = copy_array(envp);
		if (!data->envp)
			return (1);
		data->envp_export = copy_array_prefix(envp, 0, 0, 0);
		if (!data->envp_export)
			return (ft_free_split(data->envp), 1);
		sort_array(data->envp_export);
	}
	init_history(history);
	if (signal_init())
		return (1);
	return (0);
}

static void	free_all(t_data *data, t_history *history)
{
	if (data->str)
		free(data->str);
	if (data->fd[0] != 0)
		close(data->fd[0]);
	if (data->fd[1] != 1)
		close(data->fd[1]);
	ft_free_split(data->envp);
	ft_free_split(data->envp_export);
	destroy_history(history);
}

// debug(data->str); to be removed?
// data->readline_switch = 0: to check whether the readline
// in here_doc returns NULL for that stupid empty line
// write (2, "exit\n", 5); not really printing to STDERR??
// calloc: copy one more \0 to imitate gnl
// to avoid invalid read in Ka Hou's code
static int	read_input(t_data *data)
{
	char	*temp;

	data->readline_switch = 0;
	temp = readline("minishell> ");
	if (g_sigrecv != 0)
	{
		data->exit_code = 130;
		g_sigrecv = 0;
	}
	if (!temp)
	{
		write(2, "exit\n", 5);
		return (1);
	}
	signal_init();
	data->str = ft_calloc(ft_strlen(temp) + 2, 1);
	if (!data->str)
		return (1);
	ft_strlcpy(data->str, temp, ft_strlen(temp) + 1);
	free(temp);
	debug(data->str);
	return (0);
}

/* static int	take_arg(t_data *data, t_history *history, char *input)
{
	if (input && input[0] == '\0')
		return (free_all(data, history), 0);
	data->str = ft_calloc(ft_strlen(input) + 2, 1);
	if (!data->str)
		return (free_all(data, history), perror("calloc failed"), 1);
	ft_strlcpy(data->str, input, ft_strlen(input) + 1);
	make_tree(data);
	free(data->str);
	free_all(data, history);
	return (data->exit_code);
} */

/**
 * @brief Main function of the program
 *
 * This is the main function and entry point of the program.
 * As a wise man once said: "This is where the magic happens!"
 *
 * @author kfan
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @param envp Environment
 * @return int
 * @retval exit_code Exit code of the program.
 */
int	main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_history	history;

    (void)argc;
    (void)argv;
	if (init_main(&data, envp, &history))
		return (1);
/* 	if (argc == 3 && str_equals(argv[1], "-c") && argv[2])
		return (take_arg(&data, &history, argv[2])); */
	while (1)
	{
		if (read_input(&data))
			break ;
		make_tree(&data);
		history_add(&history, data.str);
		data.str = NULL;
		if (data.error == 2)
			break ;
	}
	free_all(&data, &history);
	return (data.exit_code);
}
