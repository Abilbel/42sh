/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules_statement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 13:23:08 by gchainet          #+#    #+#             */
/*   Updated: 2019/02/11 22:42:40 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

int	rule_create_statement(t_parser *parser, t_ast_token *list)
{
	(void)parser;
	list->type = TT_STATEMENT;
	return (0);
}

int	rule_pop_cmd_statement(t_parser *parser, t_ast_token *list)
{
	t_ast_token	*new_token;

	(void)list;
	new_token = alloc_ast_token(parser->pss->ret, TT_STATEMENT);
	if (!new_token)
		return (1);
	new_token->next = parser->input_queue;
	parser->input_queue = new_token;
	pss_pop(parser);
	return (0);
}
