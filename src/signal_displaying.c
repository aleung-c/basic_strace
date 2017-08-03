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

static const char *sys_signame[] =
{
	"NULL",
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGEMT",
	"SIGFPE",
	"SIGKILL",
	"SIGBUS",
	"SIGSEGV",
	"SIGSYS",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGURG",
	"SIGCHLD",
	"SIGTSTP",
	"SIGCONT",
	"SIGSTOP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGIO",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGINFO",
	"SIGUSR1",
	"SIGUSR2"
};

int		display_signal(t_ft_strace *ft_strace, t_process *process, int *status)
{
	int				stop_sig;
	siginfo_t		sig_info;

	(void)ft_strace;
	(void)process;
	stop_sig = WSTOPSIG(*status);
	if (stop_sig != 133)
	{
		ptrace(PTRACE_GETSIGINFO, process->pid, NULL, &sig_info);

		printf("\n--- %s {si_signo=%s, si_code=%d, si_pid=%d, si_uid=%d} ---\n",
			sys_signame[stop_sig],
			sys_signame[stop_sig],
			sig_info.si_code,
			sig_info.si_pid,
			sig_info.si_uid);
		if (stop_sig == SIGCHLD || stop_sig == SIGCLD)
		{
			ft_strace->in_syscall = 0;
			return (0);
		}
		else if (stop_sig == SIGSEGV)
		{
			printf("+++ killed by SIGSEGV +++\n");
			return (-1);
		}
		else if (stop_sig == SIGABRT)
		{
			return (-1);
		}
		else if (stop_sig == SIGSTOP)
		{
			ft_strace->in_syscall = 0;
			return (0);
		}
		else if (stop_sig == SIGWINCH)
		{
			printf("rt_sigreturn() = 0\n"); // simulated
			ft_strace->in_syscall = 0;
			return (0);
		}
		else
		{
			printf("\n--- %s ---\n", strsignal(stop_sig));
			return (0);
		}
	}
	return (0);
}
