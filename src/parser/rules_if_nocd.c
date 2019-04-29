/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules_if_nocd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchainet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/31 11:53:35 by gchainet          #+#    #+#             */
/*   Updated: 2019/01/09 18:31:30 by gchainet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "shell.h"
#include "ast.h"

int	rule_create_if_nocd(t_parser *parser, t_token *list)
{
	t_ast	*node;

	node = alloc_ast(NULL, TT_IF, &exec_if, &free_if);
	if (!node || pss_push(parser, PS_IFNOCD | PS_NONE))
		return (1);
	parser->pss->ret = node;
	shift_token(parser, list, 1);
	return (0);
}

int	rule_if_add_cd(t_parser *parser, t_token *list)
{
	if (parser->pss->ret->data)
		return (1);
	parser->pss->ret->data = list->data;
	shift_token(parser, list, 0);
	return (0);
}

int	rule_if_close_cd(t_parser *parser, t_token *list)
{
	if (!parser->pss->ret->data)
		return (1);
	parser->pss->state = PS_IFCD | PS_NONE;
	shift_token(parser, list, 1);
	return (0);
}
