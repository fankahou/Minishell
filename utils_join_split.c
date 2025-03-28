/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_join_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/03/26 20:22:40 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void join_split2(char **dest, char **src, char **temp, int strlen)
{
    int k;
    int i;
    
    if (!temp)
        return ;
    k = 0;
    while (dest[k])
    {
        temp[k] = dest[k];
        k++;
    }
    i = 0;
    while (k < strlen)
    {
        temp[k] = src[i];
        k++;
        i++;
    }
}

char **join_split(char ***old, char ***cmd, int i, int j)
{
    char **src;
    char **dest;
    char **temp;

    dest = *old;
    src = *cmd;
    while (src[i])
        i++;
    while (dest[j])
        j++;
    temp = malloc(sizeof(char *) * (i + j + 1));
    if (!temp)
        perror("malloc failed");
    else
        temp[i + j] = NULL;
    join_split2(dest, src, temp, i + j);
    free(src);
    free(dest);
    *old = NULL;
    *cmd = NULL;
    return (temp);
}
