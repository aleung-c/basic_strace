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
	t_ft_strace		ft_strace;
	t_process		*process;

	(void)argc;
	init_syscall_list(&ft_strace);
	if (argv[1])
	{
		process = get_process(argv[1]);
		if (process)
		{
			printf(KGRN "- tracing %s%s\n", process->path, KRESET);
			trace_process(&ft_strace, process, argc, argv);
		}
	}
}

/*
**	This is where ill fork the main program to the process and
**	follow it with ptrace();
*/

// TODO !! : Use sigprocmask or the grade will be 0 !
// => must catch in the tracer every signal that can stop the tracer!
// SIGKILL and SIGSTOP cant be caught.

void			trace_process(t_ft_strace *ft_strace, t_process *process, int argc, char **argv)
{
	int			pid;
	int			status;
	sigset_t	empty;
	sigset_t	new_sigset;

	(void)argc;
	pid = fork();
	if (pid == 0)
	{
		// Child process
		// need to ptrace the new process then execve the said exec.
		// close(1); // silence prints from the child.
		raise(SIGSTOP);
		execve(process->path, argv + 2, NULL);
	}
	else
	{
		// prepare signal catching in the tracer.
		sigemptyset(&new_sigset);
		// empty the set
		sigemptyset(&empty);
		

		sigaddset(&new_sigset, SIGHUP);
		sigaddset(&new_sigset, SIGINT);
		sigaddset(&new_sigset, SIGQUIT);
		sigaddset(&new_sigset, SIGPIPE);
		sigaddset(&new_sigset, SIGTERM);
		sigaddset(&new_sigset, SIGBUS);

		sigprocmask(SIG_SETMASK, &empty, NULL);
		sigprocmask(SIG_BLOCK, &new_sigset, NULL);

		process->pid = pid;
		// Parent process. pid var has the pid of child.
		ptrace(PTRACE_SEIZE, pid, NULL, PTRACE_O_TRACESYSGOOD);
		ptrace(PTRACE_INTERRUPT, pid, 0, 0);
		while (1)
		{
			
			
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			waitpid(-1, &status, WUNTRACED);
			if (WIFEXITED(status))
			{
				if (WIFSIGNALED(status))
				{
					printf("terminated by signal!\n");
				}
				break;
			}
			if (WIFSTOPPED(status))
			{
				// a stop may come from the ptrace(value == 133), or
				// from any signal. The check is made in those functions.
				display_syscall(ft_strace, process);
				display_signal(ft_strace, process, &status);
			}
		}
	}
}
