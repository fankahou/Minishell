/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/04/07 17:30:45 by kmautner         ###   ########.fr       */
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
 * @param info information about the source of the signal
 * @param context context (ignored)
 *
 * @author kmautner
 */
void	signal_handler(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signal == SIGSEGV)
	{
		ft_printf("You fucking moron, you caused a segmentation fault! >:(\n");
		exit(0);
	}
	if (g_sigrecv != 0)
		debug("Unprocessed signal in sigrecv! This signal will be overwritten!");
	g_sigrecv = signal;
	if (g_sigrecv == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
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

	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGINT, &act, NULL))
		return (error("Error setting sigaction for SIGINT"));
	if (sigaction(SIGQUIT, &act, NULL))
		return (error("Error setting sigaction for SIGQUIT"));
	if (sigaction(SIGSEGV, &act, NULL))
		return (error("Error setting sigaction for SIGSEGV"));
	return (0);
}
