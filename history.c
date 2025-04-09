/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:09:49 by kmautner          #+#    #+#             */
/*   Updated: 2025/04/09 16:40:34 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Add an entry to the history.
 *
 * This function adds a malloc'd string to the history.
 * If an equal string is already the latest entry in history,
 * it will get free'd and ignored.
 *
 * @param history history struct
 * @param command entry to add to history
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kmautner
 */
int	history_add(t_history *history, char *command)
{
	debug("Adding new element to history...");
	if (command[0] == '\n' || command[0] == '\0')
		return (free(command), debug("Will not add empty line to history."), 0);
	if (history->length > 0 && str_equals(history->list[history->length - 1],
			command))
	{
		return (free(command), debug("Command is already latest entry."), 0);
	}
	if (history->length >= HISTORY_MAX_SIZE)
	{
		shift_array(history->list, history->length);
		history->length--;
	}
	history->list[history->length] = command;
	history->length++;
	add_history(command);
	return (0);
}

/**
 * @brief Reads history file into history.
 *
 * This funciton is used to open the history file and add its
 * contents to the current history. Should the file not exist
 * or be unopenable this function is skipped. Should the reading
 * of the file fail it will stop execution at that point. Any
 * entries after the fail will not get loaded into memory.
 *
 * @param history history struct
 * @param fd file descriptor of the opened history file
 * @return int
 * @retval success 0 on success, 1 on error.
 *
 * @author kmautner
 */
int	read_history_file(t_history *history, int fd)
{
	char	*command;

	if (fd < 0)
		return (0);
	debug("Reading history from file...");
	command = get_next_line(fd);
	while (command)
	{
		if (command[0] == '\0')
			return (free(command), 0);
		if (command[ft_strlen(command) - 1] == '\n')
			command[ft_strlen(command) - 1] = '\0';
		if (command[0] != '\n' && command[0] != '\0')
		{
			if (history_add(history, command))
				warn("Error adding a command from file!");
		}
		command = get_next_line(fd);
	}
	get_next_line(-1);
	return (0);
}

/**
 * @brief Initialise the history of the minishell.
 *
 * Function used to initialise the static data of
 * the history. If the history file is present,
 * its contents get read into the history using
 * read_history_file().
 *
 * @param history history struct pointer
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @ref read_history_file
 *
 * @author kmautner
 */
int	init_history(t_history *history)
{
	int	fd;

	if (!history)
		return (warn("invalid history pointer!"), 0);
	debug("Initialising history...");
	ft_bzero(history, sizeof(t_history));
	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd < 0)
		return (warn("Could not open history file!"), 0);
	if (read_history_file(history, fd))
		return (warn("Error reading history file!"), 0);
	return (close(fd), 0);
}

/**
 * @brief Writes the current history to the history file.
 *
 * This function opens the history file, truncates it and writes
 * the current history to the file. Should the file not exist it will
 * be created. If the file cannot be opened execution will stop and
 * the current history will get lost. Write fails are ignored and may
 * lead to a corrupted file.
 *
 * @param history history struct
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kmautner
 */
int	history_write(t_history history)
{
	int	fd;
	int	c;

	fd = open(HISTORY_FILE, O_CREAT | O_TRUNC | O_RDWR, 0777);
	if (fd < 0)
		return (error("Error opening history file!"));
	c = 0;
	debug("----- HISTORY -----");
	while (c < history.length)
	{
		debug(history.list[c]);
		write(fd, history.list[c], ft_strlen(history.list[c]));
		write(fd, "\n", 1);
		c++;
	}
	return (close(fd), 0);
}

/**
 * @brief Destroys the history of the minishell.
 *
 * This function destroys the current history.
 * All entries are written to the history file using
 * history_write() before the history is destroyed.
 *
 * @param history history struct
 *
 * @ref history_write
 *
 * @author kmautner
 */
void	destroy_history(t_history *history)
{
	int	c;

	debug("Destroying history...");
	if (history_write(*history))
		error("Error writing history!");
	c = 0;
	while (c < history->length)
	{
		free(history->list[c]);
		c++;
	}
}
