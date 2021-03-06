/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/15 09:46:52 by gchainet          #+#    #+#             */
/*   Updated: 2019/05/03 00:21:51 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "jobs.h"
#include "ast.h"

int		exec_end(t_shell *shell, t_ast *ast)
{
	if (shell->ctrlc)
		return (0);
	if (ast->left)
	{
		if (exec_job(shell, ast->left, NULL))
			return (-1);
		if (shell->ctrlc)
			return (0);
	}
	if (ast->right)
		return (exec_job(shell, ast->right, ast->job));
	return (0);
}
