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
	int				i;

	ft_strace->syscall_list.list = NULL;
	ft_strace->syscall_list.list = (char ***)malloc(sizeof(char **) * SYSCALLS_NB);
	if ((fp = fopen("./ressources/syscall_list.csv", "r")) == NULL)
	{
		printf("File opening error. Syscall list not found\n");
		exit(-1);
	}
	else
	{
		buf = NULL;
		buf_size = 0;
		line_number = 0;
		while ((getline(&buf, &buf_size, fp))
				&& line_number != SYSCALLS_NB)
		{
			original_buf = buf;
			ft_strace->syscall_list.list[line_number] =
				(char **)malloc(sizeof(char *) * SYSCALLS_NB_COL);

			// init each syscall subvalue to null.
			for (i = 0; i != SYSCALLS_NB_COL; i++)
			{
				ft_strace->syscall_list.list[line_number][i] = NULL;
			}

			// Actual line parsing. CSV format --> separator is the ','
			word = strtok(buf, ",");
			for (i = 0; word != NULL; word = strtok(NULL, ",\n"), i++)
			{
				if (strncmp(word, "NULL", 4) != 0)
				{
					// printf("word = [%s] \n", word);
					ft_strace->syscall_list.list[line_number][i] =
						(char *)malloc(sizeof(char) * strlen(word) + 1);
					ft_strace->syscall_list.list[line_number][i][strlen(word)] = '\0';
					strncpy(ft_strace->syscall_list.list[line_number][i], word, strlen(word));
				}
			}
			free(original_buf);
			buf = NULL;
			line_number++;
			// printf("one line done #%d\n", line_number);
			// sleep(10);
		}
		free(buf);
		fclose(fp);
		// printf(KBLU "Scooped every syscall!\n" KRESET);
		return ;
	}
	return ;


}
