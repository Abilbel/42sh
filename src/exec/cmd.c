/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/15 09:43:54 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/07 22:15:38 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "21sh.h"
#include "ast.h"

int		exec_cmd(t_shell *shell, t_ast *ast)
{
	int	ret;

	redir_save(ast->redir_list, ast);
	prepare_redirs(shell, ast, ast);
	ret = exec(shell, ast);
	reset_redirs(ast);
	return (ret);
}

void	free_cmd(t_ast *ast)
{
	free_command(ast->data);
	free_ast(ast);
}
