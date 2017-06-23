/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/23 18:42:20 by aleung-c          #+#    #+#             */
/*   Updated: 2017/06/23 18:42:21 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

/*
**	Parse the arg string and fill the process struct,
**	then return said struct.
*/

t_process		*get_process(char *arg)
{
	t_process	*process;

	if (access(arg, F_OK|X_OK) == 0)
	{
		process = (t_process *)malloc(sizeof(t_process));
		asprintf(&process->path, "%s", arg);
	}
	else
	{
		perror("access");
		process = NULL;
	}
	return (process);
}
