/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_displaying.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 17:01:10 by aleung-c          #+#    #+#             */
/*   Updated: 2017/08/01 17:01:11 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

void		display_signal(t_ft_strace *ft_strace, t_process *process, int *status)
{
	int		stop_sig;

	(void)ft_strace;
	(void)process;
	stop_sig = WSTOPSIG(*status);
	// if (stop_sig != 133)
	// {
		if (stop_sig == SIGCHLD || stop_sig == SIGCLD)
		{
			printf("\n--- SIGCHILD ---\n");
		}
		else if (stop_sig == SIGSEGV)
		{
			printf("\n--- SIGSEGV ---\n");
		}
		else if (stop_sig == SIGABRT)
		{
			printf("\n--- SIGSEGV ---\n");
		}
	// }
}
