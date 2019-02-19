/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/13 08:45:36 by gchainet          #+#    #+#             */
/*   Updated: 2019/02/19 10:09:24 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "shell.h"
#include "fill_line.h"
#include "libft.h"

static int	exit_value(t_shell *shell, char **args)
{
	int		ret;
	char	*str;

	if (args[1])
		str = ft_strdup(args[1]);
	else
		str = NULL;
	free_shell(shell);
	if (str && !ft_isdigit(str[0]) && str[0] != '-'
			&& str[0] != '+')
	{
		free(str);
		exit(255);
	}
	if (str)
	{
		ret = ft_atoi(str);
		free(str);
		exit(ret);
	}
	else
		exit(0);
}

int			builtin_exit(t_shell *shell, char **args)
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
	if (args[1])
		ft_dprintf(shell->fd_hf, "exit %s\n", args[1]);
	else
		ft_dprintf(shell->fd_hf, "exit\n");
	if (close(shell->fd_hf) == -1)
		ft_dprintf(2, "Error on closing the history file\n");
	if (close(shell->fd_op) == -1)
		ft_dprintf(2, "Error on closing the tty fd\n");
	return (exit_value(shell, args));
}
