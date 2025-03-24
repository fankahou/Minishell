/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:42:29 by kfan              #+#    #+#             */
/*   Updated: 2024/10/22 14:26:04 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_memdup(char *s, size_t size)
{
	char	*str;

	str = ft_calloc((size + 1), sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s, size);
	return (str);
}

static char	*ft_free(char **buffer)
{
	if (*buffer)
		free(*buffer);
	*buffer = NULL;
	return (NULL);
}

static char	*ft_alloc(char *buffer, char **ptr)
{
	char	*newbuffer;
	int		i;

	i = 0;
	if (buffer)
	{
		while (buffer[i] != '\0')
			i++;
		newbuffer = ft_calloc((i + BUFFER_SIZE + 1), sizeof(char));
		if (!newbuffer)
			return (ft_free(ptr));
		ft_memcpy(newbuffer, buffer, i);
		free(buffer);
		buffer = NULL;
		return (newbuffer);
	}
	else
	{
		newbuffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
		if (!newbuffer)
			return (ft_free(ptr));
		return (newbuffer);
	}
}

static char	*ft_return(char **buffer, int i)
{
	char	*str;
	int		j;
	char	*temp;

	str = ft_memdup(*buffer, i);
	if (!str)
		return (ft_free(buffer));
	temp = *buffer;
	j = 0;
	while (temp[i + j] != '\0')
		j++;
	*buffer = ft_memdup(&temp[i], j);
	free(temp);
	temp = NULL;
	if (!(*buffer))
	{
		free(str);
		str = NULL;
		return (NULL);
	}
	return (str);
}

char	*get_next_line(int fd)
{
	int			i;
	int			word;
	static char	*buffer;
	char		**ptr;

	ptr = &buffer;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free(ptr));
	buffer = ft_alloc(buffer, ptr);
	if (!buffer)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i])
		i++;
	if (buffer[i] == '\n')
		return (ft_return(ptr, i + 1));
	word = read(fd, &buffer[i], BUFFER_SIZE);
	if (word < 0)
		return (ft_free(ptr));
	if (word == 0 && buffer[0] == '\0')
		return (ft_free(ptr));
	else if (word == 0 && buffer[i] == '\0')
		return (ft_return(ptr, i));
	return (get_next_line(fd));
}

/* #include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*str;
	int		i;

	i = 0;
	fd = open("1char.txt", O_RDONLY);
	while (i < 10)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		printf("%s", str);
		//printf("--------------------\n");
		free(str);
		i++;
	}
	return (0);
}
 */