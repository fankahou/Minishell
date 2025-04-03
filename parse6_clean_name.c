/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse6_clean_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/03 19:13:45 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//if the next one is also space it will check if it's the last space
//and return 2 to stop copying that space
//otherwise return 1 for copy the space
static int get_rid_of_extra_space(char *temp)
{
    int i;
    
    if (is_space(temp[1]) || temp[1] == '\0')
    {
        i = 0;
        while (is_space(temp[i]))
            i++;
        if (temp[i] == '\0')
            return (2) ;
    }
    return (1);
}

static int clean_name_envp(char *temp, t_token *token, t_clean *clean)
{
    int i;
    char **check;
    
    clean->envp_temp = expand_envp(&temp[clean->count + 1], token, NULL, clean);
    if (!clean->envp_temp)
        return (token->error[0] = 1, perror("expand_envp failed"), free(clean->file), -1);
    clean->count = clean->count + check_envp_count(&temp[clean->count + 1]);
    if (ft_strlen(clean->envp_temp) > 0)
        clean->space = 0;
    clean->new = ft_strjoin(clean->file, clean->envp_temp);
    free(clean->file);
    clean->file = NULL;
    if (!clean->new)
        return (free(clean->envp_temp), perror("ft_strjoin failed\n"), -1);
    check = ft_split_space(clean->envp_temp);
    if (!check)
        perror("ft_split_space failed");
    else
    {
        i = 0;
        while (check[i])
            i++;
        //printf("i = %d\n", i);
        if ((i == 0 && ((clean->envp_temp[0] != '\0') || clean->quote != 2) && clean->new[0] == '\0' ) || i > 1)
            clean->temp = ft_cmd(clean->new, token, NULL);
        ft_free_split(check);
    }
    free(clean->envp_temp);
    return (0);
}

static int clean_name_char(char *temp, t_clean *clean)
{
    if (is_space(temp[clean->count]))
        clean->space = clean->space + get_rid_of_extra_space(&temp[clean->count]);
    else
        clean->space = 0;
    if ((clean->space < 2 || clean->quote > 0) && (!is_quote(temp[clean->count]) || clean->quote > 2))
        clean->new = ft_charjoin(clean->file, &temp[clean->count]);
    else
        clean->new = ft_strdup(clean->file); //
    free(clean->file);
    clean->file = NULL;
    if (!clean->new)
        return (perror("ft_charjoin failed"), -1);
    return (0);
}

static void    init_clean(t_clean *clean, int count, char *file)
{
    clean->quote = 0;
    clean->space = 0;
    clean->count = count;
    clean->envp_temp = NULL;
    clean->new = NULL;
    clean->file = file;
    clean->temp = NULL;
}

//  $USER inside ' ' shouldnt expand, otherwise always expand
// get rid of extra space if not inside quote
// count == 0 for cmd, otherwise for redir
// only create new cmd array if token->pipe == 0???
char *clean_name(char *temp, t_token *token, int count, char *file)
{
    t_clean clean;
    int return_val;

    while (is_space(temp[count]))
        count++;
    init_clean(&clean, count, file);
    return_val = 0;
    while (temp[clean.count])
    {
        if (clean.new)
            clean.file = clean.new;
        clean.quote = inside_quote(temp[clean.count], clean.quote);
        if (temp[clean.count] == '$' && clean.quote != 1 && clean.quote != 3)
            return_val = clean_name_envp(temp, token, &clean);
        else
            return_val = clean_name_char(temp, &clean);
        if (return_val == -1)
            return (NULL);
        clean.count++;
    }
    // split here?
    if (!clean.new && clean.file)
        free(clean.file);
    if (clean.quote > 0) // not nessecary? just pass it to execve?
        return (syntax_error("unexpected EOF while looking for matching quote", token), clean.new);
    if (clean.temp)
        token->data->cmd_temp = clean.temp;
    return (clean.new);
}
