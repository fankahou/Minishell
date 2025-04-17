/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1_make_tree.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endermenskill <endermenskill@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/17 17:38:04 by endermenski      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check for errors and count the amount of token arrays needed
 *
 * It will throw error if two delimiters are next to each other (eg: "; ;"
 * or "; &&" or "|| &&")
 * error_count starts at 0 and will increment whenever it hits a delimiter.
 * If it hits a non-delimiter it resets and it throws an error when it goes
 * over 1.
 *
 * @author kfan
 *
 * @param temp array of split of the initial str
 * @param error_count Counting variable (always set to 0!)
 * @return int
 * @retval amount Number of arrays needed for malloc the **token.
 */
static int	check_temp(char **temp, int error_count, int k)
{
	int	i;
	int	j;

	i = 0;
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
			return (syntax_error(&temp[i][j], NULL), 0);
		i++;
	}
	if (i > 0 && is_delimiter(temp[i - 1][j], temp[i - 1][j + 1]) > 1)
		return (syntax_error("unexpected end of file", NULL), 0);
	return (k);
}

/**
 * @brief Creates a token.
 *
 * Creates an array of token structs and
 * initialises them with the commands inside
 * temp and the data inside data
 *
 * @author kfan
 *
 * @param temp command list
 * @param token Array of tokens
 * @param data data struct pointer
 * @return t_token**
 * @retval token New token or NULL on error.
 */
static t_token	**make_token(char **temp, t_token **token, t_data *data)
{
	int	i;

	i = check_temp(temp, 0, 0);
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
 * new: reset fd after all pipe, technically force update fd
 * new: wait here instead of in pipex parent
 *
 * @author kfan
 *
 * @param token Array of tokens to execute
 * @param data Data struct containing needed information
 * @param fd Array od duplicated file descriptors
 * @param i Just a counter starting with 0
 * @return int
 * @retval success Always returns 0.
 */
static int	execute(t_token **token, t_data *data, int *fd, int i)
{
	while (token[i] && data->error == 0)
	{
		token[i]->envp = data->envp;
		token[i]->envp_export = data->envp_export;
		if (clean_and_expand(token[i]))
			break ;
		signal_init_execve();
		if (token[i]->nmb_of_cmd > 0)
			pipex(token[i]);
		restore_fd(data, fd);
		wait_pipes(token[i], 0, 0);
		while (token[i] && token[i]->delimiter == 2 && token[i]->exit_code[0] != 0)
			i++;
		while (token[i] && token[i]->delimiter == 3 && token[i]->exit_code[0] == 0)
			i++;
		if (token[i])
			i++;
	}
	if (data->readline_switch == 0)
		signal_init();
	else
		signal_init1();
	return (0);
}

/**
 * @brief Initializes the data for token processing.
 *
 * This function initializes the tree creation process.
 * It duplicates STDIN and STDOUT using dup() and writes
 * them into fd[0] and fd[1] respectively.
 *
 * @author kfan
 *
 * @param data pointer to data struct
 * @param fd pointer to duplicate file descriptors into
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
static int	init_tree(t_data *data, int *fd)
{
	fd[0] = dup(data->fd[0]);
	if (fd[0] == -1)
		return (perror("dup failed"), 1);
	fd[1] = dup(data->fd[1]);
	if (fd[1] == -1)
		return (close(fd[0]), perror("dup failed"), 1);
	data->fd_in = fd[0];
	data->fd_out = fd[1];
	data->error = 0;
	data->cmd_temp = NULL;
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
 * @author kfan
 *
 * @param data data to parse into the tokens
 */
void	make_tree(t_data *data)
{
	char	**temp;
	t_token	**token;
	int		fd[2];

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
		if (token)
			execute(token, data, fd, 0);
		if (token)
			close_unused_fd(token, 0);
		if (token)
			ft_free_token(token);
	}
	restore_fd(data, fd);
	close(fd[0]);
	close(fd[1]);
}
