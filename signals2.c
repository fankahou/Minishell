/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/04/10 16:24:59 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Signal handler for heredocs.
 *
 * Modified version of the signal handler for heredocs.
 * 
 * @param signal signal number
 * @param info signal info (ignored)
 * @param context signal context (ignored)
 *
 * @ref signal_handler
 *
 * @author kfan
 */
void	signal_handler_here_doc(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	g_sigrecv = signal;
	if (g_sigrecv == SIGINT) // need to quit here_doc!!!
	{
		write(2, "\n", 1); // how about execve??
		close(0); // really?? KA HOU: I close the STD_IN, works like eof, but I dont know whether it's correct...
	}
	if (signal == SIGSEGV)
		exit(0);
}

/**
 * @brief Initialiser for the modified signal handlers.
 * 
 * Inistialiser to overwrite the standard signal handlers
 * with the modified ones for heredoc.
 *
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
int	signal_init_here_doc(void)
{
	struct sigaction	act;

	struct sigaction ignore; // KA HOU: new: set SIGQUIT to ignore
	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler_here_doc;
	act.sa_flags = SA_SIGINFO;// | SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigemptyset(&ignore.sa_mask);
	if (sigaction(SIGINT, &act, NULL))
		return (error("Error setting sigaction for SIGINT"));
	if (sigaction(SIGTERM, &act, NULL))
		return (error("Error setting sigaction for SIGTERM"));
	if (sigaction(SIGQUIT, &ignore, NULL))
		return (error("Error setting sigaction for SIGTERM"));
	if (sigaction(SIGSEGV, &act, NULL))
		return (error("Error setting sigaction for SIGSEGV"));
	return (0);
}

/**
 * @brief Modified signal handler for execve.
 *
 * Modified signal handler for execve because stupid.
 * 
 * @param signal signal number
 * @param info signal info (unused)
 * @param context signal context (unused)
 *
 * @ref signal_handler
 *
 * @author kfan
 */
void	signal_handler_execve(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	g_sigrecv = signal;
	if (g_sigrecv == SIGINT)
	{
		write(2, "\n", 1); // how about execve??
		//close(0); // really?? KA HOU: I close the STD_IN, works like eof, but I dont know whether it's correct...
	}
	if (signal == SIGSEGV)
		exit(0);
}

/**
 * @brief Initialises the execve signal handler.
 *
 * Initialises the modified signal handler for execve.
 * 
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 *
 * @ref signal_init
 *
 * @author kfan
 */
int	signal_init_execve(void)
{
	struct sigaction	act;
	struct sigaction ignore; // KA HOU: new: set SIGQUIT to ignore
	
	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler_execve;
	act.sa_flags = SA_SIGINFO;// | SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigemptyset(&ignore.sa_mask);
	if (sigaction(SIGINT, &act, NULL))
		return (error("Error setting sigaction for SIGINT"));
	if (sigaction(SIGTERM, &act, NULL))
		return (error("Error setting sigaction for SIGTERM"));
	if (sigaction(SIGQUIT, &ignore, NULL))
		return (error("Error setting sigaction for SIGTERM"));
	if (sigaction(SIGSEGV, &act, NULL))
		return (error("Error setting sigaction for SIGSEGV"));
	return (0);
}
