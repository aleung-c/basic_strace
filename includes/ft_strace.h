/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strace.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 16:15:55 by aleung-c          #+#    #+#             */
/*   Updated: 2017/06/22 16:15:56 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRACE_H
# define FT_STRACE_H

# define _GNU_SOURCE
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/user.h>
# include <sys/reg.h>
# include <sys/ptrace.h>
# include <errno.h>
// # include <linux/user.h>

/*
**	Color in text
*/

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define KRESET "\x1B[0m"

/*
**	Program Defines
*/

# define SYSCALLS_NB 328
# define SYSCALLS_NB_COL 9 // nb of columns for each syscall

/*
**	Program structs.
*/

enum e_syscall_col
{
	SYSCALL_NB,
	SYSCALL_RET,
	SYSCALL_NAME,
	SYSCALL_RDI,
	SYSCALL_RSI,
	SYSCALL_RDX,
	SYSCALL_R10,
	SYSCALL_R8,
	SYSCALL_R9
};

/*
** syscall format : number, then for each number, as many arg strings as needed.
*/

typedef struct						s_syscall_list
{
	char							***list;

}									t_syscall_list;

typedef struct						s_ft_strace
{
	t_syscall_list					syscall_list;
	struct user_regs_struct			regs;
}									t_ft_strace;

typedef struct						s_process
{
	char							*path;
	int								pid;
}									t_process;

/*
**	Function prototypes
*/

void								usage();

void								ft_strace(int argc, char **argv);

void								init_syscall_list(t_ft_strace *ft_strace);

t_process							*get_process(char *arg);

void								trace_process(t_ft_strace *ft_strace,
										t_process *process, int argc, char **argv);

void								display_syscall(t_ft_strace *ft_strace,
										t_process *process);

void								display_signal(t_ft_strace *ft_strace,
										t_process *process, int *status);

void								display_arg_from_type(t_ft_strace *ft_strace,
										t_process *process,
										long syscall_nb, long orig_value, int cur_arg);

int									get_user_reg_offset(int cur_arg);
long int							get_reg_from_struct(int cur_arg,
										struct user_regs_struct *regs);

#endif
