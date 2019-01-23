/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 08:45:36 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/22 11:08:45 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "shell.h"
#include "fill_line.h"
#include "libft.h"

int	builtin_exit(t_shell *shell, char **args)
{
	size_t	arg_count;

	arg_count = 0;
	while (args[arg_count])
		++arg_count;
	if (arg_count > 2)
	{
		ft_dprintf(2, "%s: exit: too many arguments\n", EXEC_NAME);
		return (1);
	}
	remove_env(shell);
	ft_dprintf(shell->fd, "exit");
	if (args[1])
		ft_dprintf(shell->fd, " %s\n", args[1]);
	else
		ft_dprintf(shell->fd, "\n");
	if (close(shell->fd) == -1)
		ft_dprintf(2, "Error on closing the history file\n");
	free_shell(shell);
	if (arg_count == 2 && !ft_isdigit(args[1][0]) && args[1][0] != '-'
			&& args[1][0] != '+')
		exit(255);
	exit(arg_count == 2 ? ft_atoi(args[1]) : 0);
}
