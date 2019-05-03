/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 14:16:35 by gchainet          #+#    #+#             */
/*   Updated: 2019/04/30 00:20:25 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "expand.h"
#include "libft.h"

static int	allocation_error(void)
{
	ft_dprintf(2, "%s: unable to allocate memory\n", EXEC_NAME);
	return (1);
}

static int	expand_expr_three_args(t_shell *shell, t_expr *expr)
{
	int		error;

	error = 0;
	if (!expr->args_value[1])
	{
		expr->args_value[1] = ft_strdup(expr->args[1]);
		if (!expr->args_value[1])
			return (1);
	}
	if (expr->args_value[0])
		free(expr->args_value[0]);
	expr->args_value[0] = expand_no_split(shell, expr->args[0], &error,
			EXP_LEXER_MASK_ALL);
	if (error)
		return (1);
	if (expr->args_value[2])
		free(expr->args_value[2]);
	expr->args_value[2] = expand_no_split(shell, expr->args[2], &error,
			EXP_LEXER_MASK_ALL);
	if (error)
		return (1);
	return (0);
}

static int	expand_expr_two_args(t_shell *shell, t_expr *expr)
{
	int		error;

	error = 0;
	if (!expr->args_value[0])
	{
		expr->args_value[0] = ft_strdup(expr->args[0]);
		if (!expr->args_value[0])
			return (1);
	}
	if (expr->args_value[1])
		free(expr->args_value[1]);
	expr->args_value[1] = expand_no_split(shell, expr->args[1], &error,
			EXP_LEXER_MASK_ALL);
	if (error)
		return (1);
	return (0);
}

int			expand_expr(t_shell *shell, t_expr *expr)
{
	if (!expr->args_value)
	{
		expr->args_value = malloc(sizeof(*expr->args_value) * expr->len);
		if (!expr->args_value)
			return (1);
		ft_bzero(expr->args_value, sizeof(*expr->args_value) * expr->len);
	}
	if (expr->len == 2)
	{
		if (expand_expr_two_args(shell, expr))
			return (allocation_error());
	}
	else if (expr->len == 3)
	{
		if (expand_expr_three_args(shell, expr))
			return (allocation_error());
	}
	return (0);
}
