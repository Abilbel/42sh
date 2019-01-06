/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/15 07:56:33 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/06 10:39:08 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "libft.h"

static const t_ast_rule g_rules[] =\
{
	{PS_ALL, {TT_WORD, 0, 0, 0}, 1, &rule_create_cmd},
	{PS_ALL, {TT_CMD, TT_WORD, 0, 0}, 2, &rule_add_to_cmd},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_R, TT_WORD, 0}, 3, &rule_redir_r},
	{PS_ALL,
		{TT_STATEMENT, TT_REDIR_R_BOTH, TT_WORD, 0}, 3, &rule_redir_r_both},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_RR, TT_WORD, 0}, 3, &rule_redir_rr},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_L, TT_WORD, 0}, 3, &rule_redir_l},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_LL, TT_WORD, 0}, 3, &rule_redir_ll},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_R_COMP, 0, 0}, 2, &rule_redir_r_comp},
	{PS_ALL, {TT_STATEMENT, TT_REDIR_R_CLOSE, 0, 0}, 2, &rule_redir_r_close},
	{PS_ALL, {TT_EXPR_OPEN, 0, 0, 0}, 2, &rule_create_expr},
	{PS_ALL, {TT_EXPR_INCOMPLETE, TT_WORD, 0, 0}, 2, &rule_add_to_expr},
	{PS_ALL, {TT_EXPR_INCOMPLETE, TT_EXPR_CLOSE, 0, 0}, 2, &rule_close_expr},
	{PS_ALL, {TT_IF, 0, 0, 0}, 1, &rule_create_if_nocd},
	{PS_ALL, {TT_WHILE, 0, 0, 0}, 1, &rule_create_while},
	{PS_IFNOCD, {TT_STATEMENT, TT_END, TT_THEN, 0}, 3, &rule_if_add_cd},
	{PS_IFNOCD, {TT_STATEMENT, TT_OVER, TT_THEN, 0}, 3, &rule_if_add_cd},
	{PS_IFCD, {TT_STATEMENT, 0, 0, 0}, 1, &rule_add_to_if},
	{PS_IFCD, {TT_ELIF, 0, 0, 0}, 1, &rule_create_elif_nocd},
	{PS_IFCD, {TT_ELSE, 0, 0, 0}, 1, &rule_create_else},
	{PS_IFCD | PS_ELSE, {TT_FI, 0, 0, 0}, 1, &rule_close_if},
	{PS_ELSE, {TT_STATEMENT, 0, 0, 0}, 1, &rule_add_to_else},
	{PS_WHILENOCD, {TT_STATEMENT, TT_END, TT_DO, 0}, 3, &rule_while_add_cd},
	{PS_WHILENOCD, {TT_STATEMENT, TT_OVER, TT_DO, 0}, 3, &rule_while_add_cd},
	{PS_WHILECD, {TT_STATEMENT, 0, 0, 0}, 1, &rule_while_add},
	{PS_WHILECD, {TT_DONE, 0, 0, 0}, 1, &rule_while_close},
	{PS_ALL, {TT_EXPR, 0, 0, 0}, 1, &rule_create_statement},
	{PS_ALL, {TT_CMD, 0, 0, 0}, 1, &rule_create_statement},
	{PS_ALL, {TT_PIPE, 0, 0, 0}, 1, &rule_pipe},
	{PS_ALL, {TT_OR, 0, 0, 0}, 1, &rule_or},
	{PS_ALL, {TT_AND, 0, 0, 0}, 1, &rule_and},
	{PS_ALL, {TT_END, TT_WORD, 0, 0}, 2, &rule_create_end},
	{PS_ALL, {TT_END, 0, 0, 0}, 1, &rule_shift_first},
	{PS_ALL, {TT_STATEMENT, TT_OVER, 0, 0}, 2, &rule_shift_second}
};

static size_t	count_tokens(t_ast_token *tokens)
{
	size_t		size;

	size = 0;
	while (tokens)
	{
		++size;
		tokens = tokens->next;
	}
	return (size);
}

static int		is_target_rule(t_ast_token *token, unsigned int rule_id,
		size_t len_tokens, int state)
{
	unsigned int	i;

	i = 0;
	if (g_rules[rule_id].len <= len_tokens)
	{
		while (i < g_rules[rule_id].len)
		{
			if (token->type != g_rules[rule_id].types[i])
				return (0);
			token = token->next;
			++i;
		}
		return (state & g_rules[rule_id].state_mask);
	}
	return (0);
}

t_ast_act		get_rule(t_ast_token *tokens, int state)
{
	size_t			len_tokens;
	unsigned int	rule;

	if (!(len_tokens = count_tokens(tokens)))
		return (NULL);
	rule = 0;
	while (rule < sizeof(g_rules) / sizeof(*g_rules))
	{
		if (is_target_rule(tokens, rule, len_tokens, state))
			return (g_rules[rule].act);
		++rule;
	}
	return (NULL);
}
