/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/03/28 19:01:28 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Append a character to a string.
 * 
 * This function joins a character to the end of a string.
 * The result is stored as a new malloc'd string and returned.
 *
 * @param s1 String to join to
 * @param s2 Character to join
 * @return char *
 * @retval ptr Malloc'd result string
 *
 * @author kfan
 */
char	*ft_charjoin(char *s1, char *s2)
{
	char	*ptr;

	if (!s1 || !s2)
		return (NULL);
	ptr = malloc(ft_strlen(s1) + 1 + 1);
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1, ft_strlen(s1) + 1);
	ft_strlcpy(ptr + ft_strlen(s1), s2, 1 + 1);
	return (ptr);
}

// used for quote switch
//quote = previous state
//return new state
// 1 = '
// 2 = "
// 3 = ' " 
// 4 = " '
int inside_quote(char c, int quote)
{
	int i;
	
	i = is_quote(c);
	if (i == 1 || i == 2)
	{
		if (quote == 0)
			return (i);
        else if (i != quote)
        {
            if ((quote == 1 && i == 2) || (quote == 3 && i == 2))
                    return (3);
            if ((quote == 2 && i == 1) || (quote == 4 && i == 1))
                    return (4);
            if ((quote == 3 && i == 1) || (quote == 4 && i == 2))
                return (0);
        }
		else
			return (0);
	}
	return (quote);
}

//also for closing unused here_doc temp file in bonus
void close_unused_fd(t_token **token, int i)
{
    int j;

    while(token && token[i])
    {
        j = 0;
        while (token[i]->cmds && token[i]->cmds[j])
        {
            if (token[i]->cmds[j]->fd[0] != token[i]->data->fd[0])
                close(token[i]->cmds[j]->fd[0]);
/*             if (token[i]->fd[1] != 1)
                close(token[i]->cmds[j]->fd[0]); */
            j++;
        }
        i++;
    }
    if (access("temp", F_OK) == 0)
        unlink("temp");
}

void restore_fd(t_data *data, int *fd)
{
    if (data->fd[0] != fd[0])
    {
        if (dup2(fd[0], data->fd[0]) == -1)
            ft_printf("dup2 failed\n"); // exit_code?
    }
    if (data->fd[1] != fd[1])
    {
        if (dup2(fd[1], data->fd[1]) == -1)
            ft_printf("dup2 failed\n");; // exit_code?
    }
}

char *join_cmd(char *src, char *new)
{
    char space[2];
    char *buffer;
    char *buffer1;
    
    if (!src)
        return (new);
    space[0] = ' ';
    space[1] = '\0';
    buffer = ft_strjoin(src, space);
    if (!buffer)
        return((perror("ft_strjoin failed\n")), new);
    free(src);
    buffer1 = ft_strjoin(buffer, new);
    if (!buffer1)
        return((perror("ft_strjoin failed\n")), buffer);
    free(buffer);
    free(new);
    return(buffer1);
}
