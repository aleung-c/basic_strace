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
**	Program structs.
*/

typedef struct						s_ft_strace
{


}									t_ft_strace;

typedef struct						s_process
{
	char							*path;
}									t_process;

/*
**	Function prototypes
*/

void								usage();
void								ft_strace(int argc, char **argv);
t_process							*get_process(char *arg);

void								trace_process(t_process *process, int argc, char **argv);

#endif
