/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/04/08 22:18:03 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @var sigrecv
 * @brief Variable to store received signals.
 *
 * This global variable contains the number of the
 * last received signal. It is exported in the header
 * and should be set to 0 after the signal has been
 * processed.
 *
 * @author kmautner
 */
int		g_sigrecv;

/**
 * @brief Signal handler to process received signals.
 *
 * This function handles signals received by minishell.
 * The signal will be stored in sigrecv. After the signal
 * has been processed, sigrecv should be set to 0!
 *
 * Remind me to remove the easter egg when we hand it in. :P
 *
 * @param signal signal number
 * @param info information about the source of the signal (ignored)
 * @param context context (ignored)
 *
 * @author kmautner
 */
void	signal_handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	// ft_printf("Received signal %i from process %i\n", signal, info->si_pid);
	/* 	if (signal == SIGSEGV)
			ft_printf("You fucking moron,
				you caused a segmentation fault! >:(\n");
			exit(0);
		}
		if (g_sigrecv != 0)
			error("Unprocessed signal in sigrecv! This signal will be overwritten!"); */
	g_sigrecv = signal;
	if (g_sigrecv == SIGINT)
	{
		write(2, "\n", 1); // why the fuck is there an extra line after here_doc ends with ctrl???
		rl_replace_line("", 0); // replace the buffer
		rl_on_new_line(); // new prompt on the next line but not shown
		rl_redisplay(); // flush the buffer
	}
}

/**
 * @brief Initlialisation function for the signal handling.
 *
 * This function initialises all static data required for the
 * signal handling.
 * Captured signals are:
 * - SIGINT
 * - SIGQUIT
 * - SIGSEGV (easter egg)
 *
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kmautner
 */
int	signal_init(void)
{
	struct sigaction	act;
	struct sigaction	ignore; // KA HOU: new: set SIGQUIT to ignore

	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO; // | SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigemptyset(&ignore.sa_mask); // no other signal to block, valgrind will complaim this if not set
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
 * @brief Signal handler for heredocs.
 *
 * Modified version of the signal handler for after heredoc returns NULL.
 * 
 * @param signal signal number
 * @param info signal info (ignored)
 * @param context signal context (ignored)
 *
 * @ref signal_handler
 *
 * @author kfan
 */
void	signal_handler1(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	g_sigrecv = signal;
	if (g_sigrecv == SIGINT) // need to quit here_doc!!!
	{
		// printf("hello\n");
		//close(0); // really?? KA HOU: I close the STD_IN, works like eof, but I dont know whether it's correct...
		// write(2, "\n", 1); // how about execve??
				rl_replace_line("", 0);
				rl_on_new_line();
				rl_redisplay();
	}
	if (signal == SIGSEGV)
		exit(0);
}

/**
 * @brief Initialiser for the modified signal handlers.
 * 
 * Inistialiser to overwrite the standard signal handlers
 * with the modified ones for after heredoc returns NULL.
 *
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 */
int	signal_init1(void)
{
	struct sigaction	act;
	struct sigaction	ignore; // KA HOU: new: set SIGQUIT to ignore
	
	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler1;
	act.sa_flags = SA_SIGINFO; // | SA_RESTART;
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
