/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/03/27 12:41:31 by kmautner         ###   ########.fr       */
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
int		sigrecv;

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
	ft_printf("Received signal %i from process %i\n", signal, info->si_pid);
	if (signal == SIGSEGV)
		ft_printf("You fucking moron, you caused a segmentation fault! >:(\n");
	if (sigrecv != 0)
		error("Unprocessed signal in sigrecv! This signal will be overwritten!");
	sigrecv = signal;
	if (signal == SIGINT)
		error("SIGINT RECEIVED!");
}

/**
 * @brief Initlialisation function for the signal handling.
 *
 * This function initialises all static data required for the
 * signal handling.
 * Captured signals are:
 * - SIGINT
 * - SIGTERM
 * - SIGSEGV (easter egg)
 *
 * @param data main data struct
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 *@author kmautner
 */
int	signal_init(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &act, NULL))
		return (error("Error setting sigaction for SIGINT"));
	if (sigaction(SIGTERM, &act, NULL))
		return (error("Error setting sigaction for SIGTERM"));
	if (sigaction(SIGSEGV, &act, NULL))
		return (error("Error setting sigaction for SIGSEGV"));
	return (0);
}
