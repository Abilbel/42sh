/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/12 08:38:43 by gchainet          #+#    #+#             */
/*   Updated: 2019/02/18 15:07:05 by cvignal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "shell.h"
#include "libft.h"

t_command	*alloc_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(*new_command));
	if (!new_command)
		return (NULL);
	if (!(new_command->args = malloc(sizeof(*new_command->args)
					* (ARGS_ALLOC_SIZE + 1))))
	{
		free(new_command);
		return (NULL);
	}
	ft_bzero(new_command->args,
			sizeof(*new_command->args) * (ARGS_ALLOC_SIZE + 1));
	new_command->alloc_size = ARGS_ALLOC_SIZE;
	new_command->args_len = 0;
	new_command->args_value = NULL;
	return (new_command);
}

static int	realloc_command(t_command *command)
{
	char			**new_args;
	unsigned int	i;

	new_args = malloc(sizeof(*new_args)
			* (command->alloc_size + ARGS_ALLOC_SIZE + 1));
	if (!new_args)
		return (1);
	i = 0;
	while (i < command->args_len)
	{
		new_args[i] = command->args[i];
		++i;
	}
	while (i < command->alloc_size + ARGS_ALLOC_SIZE + 1)
		new_args[i++] = NULL;
	free(command->args);
	command->args = new_args;
	command->alloc_size += ARGS_ALLOC_SIZE;
	return (0);
}

void		free_command(t_command *command)
{
	int		i;

	i = 0;
	while (command->args[i])
		free(command->args[i++]);
	i = 0;
	if (command->args_value)
	{
		while (command->args_value[i])
			free(command->args_value[i++]);
		free(command->args_value);
	}
	free(command->args);
	free(command);
}

int			add_to_command(t_command *command, char *word)
{
	if (command->args_len + 1 >= command->alloc_size)
	{
		if (realloc_command(command))
			return (1);
	}
	command->args[command->args_len++] = word;
	return (0);
}
