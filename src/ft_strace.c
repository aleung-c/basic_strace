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

void			trace_process(t_ft_strace *ft_strace, t_process *process, int argc, char **argv)
{
	int			pid;
	int			status;

	(void)ft_strace;
	(void)argc;
	pid = fork();
	if (pid == 0)
	{
		// Child process
		// need to ptrace the new process then execve the said exec.
		raise(SIGSTOP);
		execve(process->path, argv + 2, NULL);
	}
	else
	{
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
				break;
			}
			if (WIFSTOPPED(status))
			{
				if (WIFSIGNALED(status))
				{
					printf("signal stop with %d!\n",  WTERMSIG(status));
				}
				else
				{
					display_syscall(ft_strace, process);
				}
			}
		}
	}
}
