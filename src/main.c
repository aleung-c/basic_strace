/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleung-c <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 16:14:10 by aleung-c          #+#    #+#             */
/*   Updated: 2017/06/22 16:14:13 by aleung-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_strace.h"

int		main(int argc, char **argv)
{
	printf("Hello ft_strace!\n");
	if (argc > 1)
	{
		ft_strace(argc, argv);
	}
	else
	{
		usage();
	}
	return (0);
}


void	usage()
{
	printf("Usage: ./ft_strace [BIN]\n");
}
