/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:49:39 by kmautner          #+#    #+#             */
/*   Updated: 2025/03/26 17:14:29 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signal	sigrecv;

/**
 * @brief Signal handler to process received signals.
 *
 * This function handles signals received by minishell.
 * Remind me to remove the easter egg when we hand it in.
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
	if (sigrecv.check)
	{
		error("Unprocessed signal in signal storage!");
		return ;
	}
	sigrecv.signal = signal;
	sigrecv.siginfo = info;
	sigrecv.context = context;
	sigrecv.check = 1;
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
 * @return 0 on success, 1 otherwise.
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
