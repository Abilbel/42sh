/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ne.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/28 16:22:17 by gchainet          #+#    #+#             */
/*   Updated: 2018/12/28 16:23:11 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"

int	expr_ne(t_shell *shell, char **args)
{
	(void)shell;
	if (ft_atoi(args[0]) != ft_atoi(args[2]))
		return (0);
	return (1);
}
