/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_syscall_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/25 17:56:44 by aleung-c          #+#    #+#             */
/*   Updated: 2017/07/25 17:56:47 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

/*
**	Here, ill open the ressource file with the system calls.
**	Ill put all of them into the syscall_list variable.
*/

void		init_syscall_list(t_ft_strace *ft_strace)
{
	FILE			*fp;
	char 			*original_buf;
	char			*buf;
	char			*word;
	size_t			buf_size;
	int				line_number;

	ft_strace->syscall_list.list = NULL;
	ft_strace->syscall_list.list = (char **)malloc(sizeof(char *) * SYSCALLS_NB);
	if ((fp = fopen("./ressources/syscall_list.txt", "r")) == NULL)
	{
		printf("File opening error. Syscall list not found\n");
		exit(-1);
	}
	else
	{
		buf = NULL;
		buf_size = 0;
		line_number = 0;
		while ((getline(&buf, &buf_size, fp)) && line_number != 313)
		{
			original_buf = buf;
			// printf("%s\n", buf);
			word = strtok(buf, "\t");
			word = strtok(NULL, "\t");
			// printf("syscall name = %s\n", word);
			ft_strace->syscall_list.list[line_number] = (char *)malloc(sizeof(char) * strlen(word) + 1);
			strncpy(ft_strace->syscall_list.list[line_number], word, strlen(word));
			free(original_buf);
			buf = NULL;
			line_number++;
			printf("one line done #%d\n", line_number);
		}
		free(buf);
		fclose(fp);
		printf("Scooped every syscall!\n");
		return ;
	}
	return ;


}
