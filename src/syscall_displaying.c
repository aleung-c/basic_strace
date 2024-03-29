/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_displaying.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/26 16:15:22 by aleung-c          #+#    #+#             */
/*   Updated: 2017/07/26 16:15:24 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

/*
**	On Debian 8.8 x64, you want to look at /usr/include/x86_64-linux-gnu/sys/user.h
**	for those user section defines.
*/

void		display_syscall(t_ft_strace *ft_strace, t_process *process)
{
	long						orig_rax; // SYSCALL_NB
	long						ret_rax; // SYSCALL_RET
	int							cur_arg;

	(void)ft_strace;
	if (ft_strace->in_syscall == 0)
	{
		// entering syscall
		ft_strace->in_syscall = 1;
		ptrace(PTRACE_GETREGS, process->pid, NULL, &ft_strace->regs);
		errno = 0;
		orig_rax = ptrace(PTRACE_PEEKUSER, process->pid,
			(sizeof(unsigned long long int) * ORIG_RAX), NULL);
		// printf("%ld\n", orig_rax);
		if (orig_rax != -1)
		{
			printf("%s(",
				ft_strace->syscall_list.list[ft_strace->regs.orig_rax][SYSCALL_NAME]);
			fflush(stdout);

			// loop for all the other, because of the ',' separating each argument.
			for (cur_arg = SYSCALL_RDI; cur_arg < SYSCALL_R9 + 1; cur_arg++)
			{
				if (ft_strace->syscall_list.list[ft_strace->regs.orig_rax][cur_arg] != NULL)
				{
					if (cur_arg != SYSCALL_RDI)
						printf(", ");
					display_arg_from_type(ft_strace, process, ft_strace->regs.orig_rax, 0, cur_arg);
				}
			}
			printf(")");
		}
	}
	else
	{
		// exiting syscall - display return val;
		orig_rax = ptrace(PTRACE_PEEKUSER, process->pid,
			(sizeof(unsigned long long int) * ORIG_RAX), NULL);
		if (orig_rax != -1)
		{
			ret_rax = ptrace(PTRACE_PEEKUSER, process->pid,
				(sizeof(unsigned long long int) * RAX), NULL);
			if (strncmp(ft_strace->syscall_list.list[orig_rax][SYSCALL_RET], "P", 1) == 0)
			{
				printf(" = %p\n", (char *)ret_rax);
			}
			else if (strncmp(ft_strace->syscall_list.list[orig_rax][SYSCALL_RET], "I", 1) == 0)
			{
				printf(" = %ld\n", ret_rax);
			}
			else if (strncmp(ft_strace->syscall_list.list[orig_rax][SYSCALL_RET], "X", 1) == 0)
			{
				printf(" = ?\n");
			}
			ft_strace->in_syscall = 0;
		}
		
	}
}

/*
**	A long is 8 bytes.
*/

void	display_arg_from_type(t_ft_strace *ft_strace, t_process *process,
								long syscall_nb, long orig_value, int cur_arg)
{
	char					*arg_type_str;
	long int				word_value;
	char					value_buffer[40];
	int						i = 0;
	int						end_found = 0;

	(void)orig_value;
	arg_type_str = ft_strace->syscall_list.list[syscall_nb][cur_arg];
	// we display according to type.
	// printf("arg_type_str = %s\n", arg_type_str);
	if (arg_type_str)
	{
		// ----- Display char pointer
		if (strstr(arg_type_str, "char *") != NULL)
		{
			if (strstr(arg_type_str, "[]") == NULL)
			{
				while (end_found == 0 && i != 5)
				{
					word_value = ptrace(PTRACE_PEEKDATA, process->pid,
						get_reg_from_struct(cur_arg, &ft_strace->regs) + (i * 8), NULL);
					memmove(value_buffer + (i * 8), &word_value, sizeof(word_value)); // moving 8 bytes
					if (memchr(value_buffer + (i * 8), 0, 8) != NULL)
					{
						end_found = 1;
					}
					i++;
				}
				printf("\"%s\"", value_buffer);
				fflush(stdout);
			}
			else
			{
				// char * array
				word_value = ptrace(PTRACE_PEEKUSER, process->pid,
					get_user_reg_offset(cur_arg) * sizeof(unsigned long long int), NULL);
				if ((char *)word_value == NULL)
				{
					printf("[NULL]");
				}
				else
				{
					printf("[%p]", (char *)word_value);
				}
				fflush(stdout);
			}
		}
		// ----- Display long int
		else if (strstr(arg_type_str, "unsigned long") != NULL)
		{
			word_value = ptrace(PTRACE_PEEKUSER, process->pid,
					get_user_reg_offset(cur_arg) * sizeof(unsigned long long int), NULL);
			if (strstr(arg_type_str, "flags") != NULL)
			{
				printf("FLAG_MASK(%ld)", word_value);
			}
			else
			{
				printf("%ld", word_value);
			}
			fflush(stdout);
		}
		// ----- Display int
		else if (strstr(arg_type_str, "int") != NULL
			||	strstr(arg_type_str, "size_t")
			||	strstr(arg_type_str, "pid"))
		{
			word_value = ptrace(PTRACE_PEEKUSER, process->pid,
				get_user_reg_offset(cur_arg) * sizeof(unsigned long long int), NULL);
			if (strstr(arg_type_str, "flags") != NULL
				|| strstr(arg_type_str, "mode") != NULL
				|| strstr(arg_type_str, "prot") != NULL)
			{
				printf("FLAG_MASK(%d)", (int)word_value);
			}
			else
			{
				printf("%d", (int)word_value);
			}
			fflush(stdout);
		}
		// ----- Display struct pointer
		else if (strstr(arg_type_str, "struct") != NULL
			&& strstr(arg_type_str, "*") != NULL)
		{
			word_value = ptrace(PTRACE_PEEKUSER, process->pid,
				get_user_reg_offset(cur_arg) * sizeof(unsigned long long int), NULL);
			if ((char *)word_value == NULL)
			{
				printf("STRUCT{NULL}");
			}
			else
			{
				printf("STRUCT{%p}", (char *)word_value);
			}
			fflush(stdout);
		}
		// ----- Display any pointer if no specialized type were found.
		else if (strstr(arg_type_str, "*") != NULL)
		{
			word_value = ptrace(PTRACE_PEEKUSER, process->pid,
				get_user_reg_offset(cur_arg) * sizeof(unsigned long long int), NULL);
			if ((char *)word_value == NULL)
			{
				printf("NULL");
			}
			else
			{
				printf("%p", (char *)word_value);
			}
			fflush(stdout);
		}
		else
		{
			printf("???"); // this should not happen.
		}
	}
}

/*
**	This is needed as what we return here is a SYSTEM DEFINE.
**	This is an association of MY arguments order
**	and the system's defines offsets.
*/

int		get_user_reg_offset(int cur_arg)
{
	if (cur_arg == SYSCALL_NAME)
		return (ORIG_RAX);
	else if (cur_arg == SYSCALL_RDI)
		return (RDI);
	else if (cur_arg == SYSCALL_RSI)
		return (RSI);
	else if (cur_arg == SYSCALL_RDX)
		return (RDX);
	else if (cur_arg == SYSCALL_R10)
		return (R10);
	else if (cur_arg == SYSCALL_R8)
		return (R8);
	else if (cur_arg == SYSCALL_R9)
		return (R9);
	return (-1);	
}

long int	get_reg_from_struct(int cur_arg, struct user_regs_struct *regs)
{
	if (cur_arg == SYSCALL_NAME)
		return (regs->orig_rax);
	else if (cur_arg == SYSCALL_RDI)
		return (regs->rdi);
	else if (cur_arg == SYSCALL_RSI)
		return (regs->rsi);
	else if (cur_arg == SYSCALL_RDX)
		return (regs->rdx);
	else if (cur_arg == SYSCALL_R10)
		return (regs->r10);
	else if (cur_arg == SYSCALL_R8)
		return (regs->r8);
	else if (cur_arg == SYSCALL_R9)
		return (regs->r9);
	return (-1);	
}