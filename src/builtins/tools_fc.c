/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_fc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvignal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 11:34:34 by cvignal           #+#    #+#             */
/*   Updated: 2019/04/28 18:53:23 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include "fill_line.h"

void	fc_exec_ast(t_shell *shell, t_token *tokens)
{
	t_ast	*ast;

	if (parse(shell, tokens) == PARSER_COMPLETE)
	{
		ast = shell->parser.ret;
		ast->exec(shell, ast);
		close_everything(shell);
		wait_loop(shell, ast);
		ast->del(ast);
		shell->parser.ret = NULL;
	}
	reset_terminal_mode(shell);
	raw_terminal_mode(shell);
}

void	fc_free_shell(t_shell *shell)
{
	t_fd	*fd;
	t_fd	*next_fd;

	free_line(&shell->line);
	lss_pop(&shell->lexer);
	pss_pop(&shell->parser);
	fd = shell->used_fd;
	while (fd)
	{
		next_fd = fd->next;
		free(fd);
		fd = next_fd;
	}
	free_token_list(shell->output);
	free_token_list(shell->current);
	if (shell->parser.ret)
	{
		shell->parser.ret->del(shell->parser.ret);
		shell->parser.ret = NULL;
	}
	exp_ss_pop(&shell->exp_lexer);
	reset_terminal_mode(shell);
}

int		fc_init_shell(t_shell *shell, t_shell *old_shell)
{
	ft_bzero(shell, sizeof(*shell));
	if (init_lexer(&shell->lexer) || init_parser(&shell->parser)
			|| init_exp_lexer(&shell->exp_lexer))
		return (1);
	shell->hash_table = old_shell->hash_table;
	if (!(shell->used_fd = (t_fd *)malloc(sizeof(t_fd))))
		return (1);
	shell->used_fd->fd = old_shell->fd_op;
	shell->used_fd->is_pipe = 0;
	shell->used_fd->next = NULL;
	ft_bzero(&shell->line, sizeof(shell->line));
	shell->vars = copy_vars(old_shell->vars, 0);
	shell->exec_vars = NULL;
	shell->fd_op = old_shell->fd_op;
	return (0);
}

int		fc_cut_pattern(t_fc *cmd, char *str)
{
	size_t	len_old;
	size_t	len_new;

	len_new = ft_strlen(ft_strchr(str, '=') + 1);
	len_old = ft_strlen(str) - len_new;
	if (!(cmd->old_p = ft_strsub(str, 0, len_old - 1)))
		return (1);
	if (!(cmd->new_p = ft_strsub(str, len_old, len_new)))
		return (1);
	return (0);
}
