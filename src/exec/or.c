/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   or.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/15 20:38:39 by gchainet          #+#    #+#             */
/*   Updated: 2019/04/25 01:00:31 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "ast.h"

int		exec_or(t_shell *shell, t_ast *ast)
{
	if (shell->ctrlc)
		return (0);
	ast->left->exec(shell, ast->left);
	wait_loop(shell, ast->left);
	set_ret(shell, ast, ast->left->ret);
	if (ast->left->ret != 0)
	{
		ast->right->exec(shell, ast->right);
		wait_loop(shell, ast->right);
		set_ret(shell, ast, ast->right->ret);
	}
	return (ast->ret);
}

void	free_or(t_ast *ast)
{
	if (ast->left)
		ast->left->del(ast->left);
	if (ast->right)
		ast->right->del(ast->right);
	free_ast(ast);
}
