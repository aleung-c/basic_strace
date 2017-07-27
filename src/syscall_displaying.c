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
	long						orig_value; // SYSCALL_NB
	int							cur_arg;
	// struct user_regs_struct		regs;
	static int					in_syscall = 0;

	if (in_syscall == 0)
	{
		// entering syscall
		in_syscall = 1;
		// ptrace(PTRACE_GETREGS, process->pid, NULL, &regs);
		// get RAX --> syscall name.
		orig_rax = ptrace(PTRACE_PEEKUSER, process->pid,
			(sizeof(unsigned long long int) * ORIG_RAX), NULL);
		if (orig_rax != -1)
			printf("%s", ft_strace->syscall_list.list[orig_rax][SYSCALL_NAME]);
		
		// get RDI --> First parameter of the function.
		orig_value = ptrace(PTRACE_PEEKUSER, process->pid,
			(sizeof(unsigned long long int) * RDI), NULL);
		printf("(");
		if (orig_value != -1 && ft_strace->syscall_list.list[orig_rax][SYSCALL_RDI])
		{
			display_arg_from_type(ft_strace, process, orig_rax, orig_value, SYSCALL_RDI);
		}

		// loop for all the other, because of the ',' separating each argument.
		for (cur_arg = SYSCALL_RSI; cur_arg < SYSCALL_R9 + 1; cur_arg++)
		{
			orig_value = ptrace(PTRACE_PEEKUSER, process->pid,
			(sizeof(unsigned long long int) * get_user_reg_offset(cur_arg)), NULL);
			if (orig_value != -1 && ft_strace->syscall_list.list[orig_rax][cur_arg] != NULL)
			{
				printf(", ");
				display_arg_from_type(ft_strace, process, orig_rax, orig_value, cur_arg);
			}
		}
		printf(")");
	
	}
	else
	{
		// exiting syscall - get return val;
		orig_rax = ptrace(PTRACE_PEEKUSER, process->pid, (sizeof(unsigned long long int) * ORIG_RAX), NULL);
		if (orig_rax != -1)
			printf(" = %ld\n", orig_rax);
		in_syscall = 0;
	}
}

/*
**	A long is 8 bytes.
*/

void	display_arg_from_type(t_ft_strace *ft_strace, t_process *process,
								long syscall_nb, long orig_value, int cur_arg)
{
	char					*arg_type_str;
	long 					str_data;
	unsigned int			i = 0;
	int						y = 0;
	int						endfound = 0;
	char					str_display[50];

	(void)process;
	arg_type_str = ft_strace->syscall_list.list[syscall_nb][cur_arg];
	// we display according to type.
	if (strstr(arg_type_str, "char *") != NULL)
	{

		i = 0;
		while (!endfound)
		{
			str_data = ptrace(PTRACE_PEEKDATA, process->pid, orig_value
						+ (i * sizeof(unsigned long)), NULL);


			while (i < sizeof(unsigned long))
			{
				if (*((char *)str_data) + i == 0)
				{
					endfound = 1;
					str_display[y] = '\0';
					break ;
				}
				str_display[y] = *((char *)str_data + i);
				y++;
				i++;
			}
			// write(1, &str_data, 64);
			printf("%s\n", str_display);
			// printf("size of long = %ld\n", sizeof(long int));
			// TODO: display 
			// int i = 0;
			// while (str_data != 0)
			// {
			// 	str_data = ptrace(PTRACE_PEEKDATA, process->pid,
			// 		orig_value + (i * 8), NULL);
			// 	write(1, (char *)str_data, 1);
			// 	i++;
			// }
			// printf("\"%s\"", (char *)orig_value); // --> NOT the right way to do that.
		}
	}
	else if (strstr(arg_type_str, "unsigned long int") != NULL)
	{
		printf("%ld", orig_value);
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