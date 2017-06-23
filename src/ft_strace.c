/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/23 18:29:51 by aleung-c          #+#    #+#             */
/*   Updated: 2017/06/23 18:29:53 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

void			ft_strace(int argc, char **argv)
{
	t_process		*process;

	(void)argc;
	if (argv[1])
	{
		process = get_process(argv[1]);
		if (process)
		{
			printf(KGRN "- tracing %s\n" KRESET, process->path);
			trace_process(process);
		}
	}
}

/*
**	This is where ill fork the main program to the process and
**	follow it with ptrace();
*/

void			trace_process(t_process *process)
{
	int		pid;

	(void)process;
	pid = fork();
	if (pid == 0)
	{
		// child process
		// need to ptrace the process then execve the said exec.
	}
	else
	{
		// parent process. will be listening with waitpid().
	}
}
