/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/03/28 18:50:00 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sort_array(char **input)
{
    char *temp;
    int val;
    int y;
    int i;

    i = 0;
    while (input[i + 1])
    {
        y = 0;
        while (input[y + 1])
        {
            val = ft_strncmp(input[y], input[y + 1], (ft_strlen(input[y]) | ft_strlen(input[y + 1])));
            if (val > 0)
            {
                temp = input[y];
                input[y] = input[y + 1];
                input[y + 1] = temp;
            }
            y++;
        }
        i++;
    }
}

char *attach_quote(char *temp)
{
    int i;
    int strlen;
    char *new;

    i = 0;
    strlen = ft_strlen(temp);
    while (temp[i] && temp[i] != '=')
        i++;
    if (temp[i] == '=')
    {
        i++;
        new = ft_calloc(strlen + 3, 1);
        if (!new)
            return (perror("ft_calloc failed"), NULL);
        ft_strlcpy(new, temp, i + 1);
        new[i] = '\"';
        ft_strlcpy(&new[i + 1], &temp[i], strlen - i + 1);
        new[strlen + 1] = '\"';
        return (new);
    }
    return (ft_strdup(temp));
}

/**
 * @brief Debug function to print an array of strings.
 *
 * Debug function to print an array of strings.
 * 
 * @param temp String array to print
 *
 * @author kfan
 */

void print_array(char **temp)
{
    int i;

    if (!temp)
        return ;
    i = 0;
    while (temp[i])
    {
        ft_printf("%s\n", temp[i]);
        i++;
    }
}
