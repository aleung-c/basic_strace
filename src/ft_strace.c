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
			printf(KGRN "- tracing %s%s\n", process->path, KRESET);
			trace_process(process, argc, argv);
		}
	}
}

/*
**	This is where ill fork the main program to the process and
**	follow it with ptrace();
*/

void			trace_process(t_process *process, int argc, char **argv)
{
	int			pid;
	long		orig_rax;
	siginfo_t	sig;

	(void)argc;
	pid = fork();
	if (pid == 0)
	{
		// child process
		// need to ptrace the new process then execve the said exec.
		ptrace(PTRACE_SEIZE, 0, NULL, NULL);
		execv(process->path, argv + 2);
	}
	else
	{
		// parent process. will be listening with waitpid().
		waitid(P_ALL, pid, &sig, 0);
		ptrace(PTRACE_INTERRUPT, pid, 0, NULL);
		orig_rax = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, NULL);
		if (orig_rax == -1)
			perror("ptrace");
		printf("The child made a system call %ld\n", orig_rax);
		ptrace(PTRACE_CONT, pid, NULL, 0);
	}
}
